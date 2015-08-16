/**
 * service.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Implements a shuttle service.
 */

// default height
var HEIGHT = 0.8;

// default latitude
var LATITUDE = 42.3745615030193;

// default longitude
var LONGITUDE = -71.11803936751632;

// default heading
var HEADING = 1.757197490907891;

// default number of seats
var SEATS = 10;

// default velocity
var VELOCITY = 50;

// global reference to shuttle's marker on 2D map
var bus = null;

// global reference to 3D Earth
var earth = null;

// global reference to 2D map
var map = null;

// global reference to shuttle
var shuttle = null;

// global  passenger's placemark & marker 
var placemarkers = [];
var markers = [];

// global score
var SCORE = 0;
// global time
var TIME = 300;

// load version 1 of the Google Earth API
google.load("earth", "1");

// load version 3 of the Google Maps API
google.load("maps", "3", {other_params: "sensor=false"});

// once the window has loaded
$(window).load(function() {

    // listen for keydown anywhere in body
    $(document.body).keydown(function(event) {
        return keystroke(event, true);
    });

    // listen for keyup anywhere in body
    $(document.body).keyup(function(event) {
        return keystroke(event, false);
    });

    // listen for click on Drop Off button
    $("#dropoff").click(function(event) {
        dropoff();
    });

    // listen for click on Pick Up button
    $("#pickup").click(function(event) {
        pickup();
    });

    // listen for click on SpeedUp button
    $("#velup").click(function(event){
	velup();
    });

    // listen for clico on Speeddown button
    $("#veldown").click(function(event){
	veldown();
    });
    
    
    // count timer
    setInterval(Score, 1000);
    
    // setTimeout(End_Time, 3000*100);
    
    
    // load application
    load();
});

// unload application
$(window).unload(function() {
    unload();
});

/**
 * Renders seating chart.
 */
function chart()
{
    // order seats by house
    /*
    var org_seats = shuttle.seats.concat();
    var houses = {};
    for (var i = 0; i < shuttle.seats.length; i++){
	if (shuttle.seats[i] != null){
	    if (houses[shuttle.seats[i].house]==null) {
		houses[shuttle.seats[i].house] = String(i);
	    } 
	    // if there is already in houses 
	    else {
		houses[shuttle.seats[i].house] += String(i);
	    }
	}
    }
    // order seats by house
    for (var key in houses){
	for (var j=0, len=length(houses[key]); j<len; j++){
	    
	}
    }
    */

    // orginal version
    var html = "<ol start='0'>";
    for (var i = 0; i < shuttle.seats.length; i++)
    {
        if (shuttle.seats[i] == null)
        {
            html += "<li>Empty Seat</li>";
        }
        else
        {
            html += "<li>" + shuttle.seats[i].name 
		+ " to " + shuttle.seats[i].house
		+ "</li>";
        }
    }
    html += "</ol>";
    $("#chart").html(html);
}

/**
 * Drops up passengers if their stop is nearby.
 */
function dropoff() {
    //  alert("TODO");
    //var nearest_dist = 10000;
    //var nearest_ind  = -1;
    var no_drop = true;
    for (var i = 0; i<SEATS; i++){
	if (shuttle.seats[i] == null) { continue; }
	var dist = shuttle.distance(HOUSES[shuttle.seats[i].house].lat, HOUSES[shuttle.seats[i].house].lng);
	if (dist <= 30.0){
	    // dropoff
	    no_drop = false;
	    SCORE += 1;
	    // announcement
	    console.log(shuttle.seats[i].name);
	    $("#announcements").html(shuttle.seats[i].name 
	    			     + " drop off.");
				     //+ "Score: " + SCORE);

	    shuttle.seats[i] = null;
	    chart();
	} 
    }
    // announcements 
    if ( no_drop ) {
	// announcement
	$("#announcements").html("There are no house to drop off people within 30m.");
		//		+ "Score: " + SCORE); 

    }
    chart();
}

/**
 * Called if Google Earth fails to load.
 */
function failureCB(errorCode) 
{
    // report error unless plugin simply isn't installed
    if (errorCode != ERR_CREATE_PLUGIN)
    {
        alert(errorCode);
    }
}

/**
 * Handler for Earth's frameend event.
 */
function frameend() 
{
    shuttle.update();
}

/**
 * Called once Google Earth has loaded.
 */
function initCB(instance) 
{
    // retain reference to GEPlugin instance
    earth = instance;

    // specify the speed at which the camera moves
    earth.getOptions().setFlyToSpeed(100);

    // show buildings
    earth.getLayerRoot().enableLayerById(earth.LAYER_BUILDINGS, true);

    // disable terrain (so that Earth is flat)
    earth.getLayerRoot().enableLayerById(earth.LAYER_TERRAIN, false);

    // prevent mouse navigation in the plugin
    earth.getOptions().setMouseNavigationEnabled(false);

    // instantiate shuttle
    shuttle = new Shuttle({
        heading: HEADING,
        height: HEIGHT,
        latitude: LATITUDE,
        longitude: LONGITUDE,
        planet: earth,
        seats: SEATS,
        velocity: VELOCITY
    });

    // synchronize camera with Earth
    google.earth.addEventListener(earth, "frameend", frameend);

    // synchronize map with Earth
    google.earth.addEventListener(earth.getView(), "viewchange", viewchange);

    // update shuttle's camera
    shuttle.updateCamera();

    // show Earth
    earth.getWindow().setVisibility(true);

    // render seating chart
    chart();

    // populate Earth with passengers and houses
    populate();
}

/**
 * Handles keystrokes.
 */
function keystroke(event, state)
{
    // ensure we have event
    if (!event)
    {
        event = window.event;
    }

    // left arrow
    if (event.keyCode == 37)
    {
        shuttle.states.turningLeftward = state;
        return false;
    }

    // up arrow
    else if (event.keyCode == 38)
    {
        shuttle.states.tiltingUpward = state;
        return false;
    }

    // right arrow
    else if (event.keyCode == 39)
    {
        shuttle.states.turningRightward = state;
	return false;
    }

    // down arrow
    else if (event.keyCode == 40)
    {
	
        shuttle.states.tiltingDownward = state;
        return false;
    }

    // A, a
    else if (event.keyCode == 65 || event.keyCode == 97)
    {
        shuttle.states.slidingLeftward = state;
        return false;
    }

    // D, d
    else if (event.keyCode == 68 || event.keyCode == 100)
    {
        shuttle.states.slidingRightward = state;
        return false;
    }
  
    // S, s
    else if (event.keyCode == 83 || event.keyCode == 115)
    {
        shuttle.states.movingBackward = state;     
        return false;
    }

    // W, w
    else if (event.keyCode == 87 || event.keyCode == 119)
    {
        shuttle.states.movingForward = state;    
        return false;
    }
  
    return true;
}

/**
 * Loads application.
 */
function load()
{
    // embed 2D map in DOM
    var latlng = new google.maps.LatLng(LATITUDE, LONGITUDE);
    map = new google.maps.Map($("#map").get(0), {
        center: latlng,
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        scrollwheel: false,
        zoom: 17,
        zoomControl: true
    });

    // prepare shuttle's icon for map
    bus = new google.maps.Marker({
        icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/bus.png",
        map: map,
        title: "you are here"
    });

    // embed 3D Earth in DOM
    google.earth.createInstance("earth", initCB, failureCB);
}

/**
 * Picks up nearby passengers.
 */
function pickup()
{
    //alert("TODO");
    for (var i = 0; i < placemarkers.length; i++) {
	var lat = placemarkers[i].pm.getGeometry().getLatitude();
	var lng = placemarkers[i].pm.getGeometry().getLongitude();
	var dist = shuttle.distance(lat, lng);
	if (dist <= 15.0
	    && HOUSES[placemarkers[i]["house"]].lat != null ) {
	    
	    // take seat
	    var seat_ok = false;
	    for (var j = 0; j < SEATS; j++) {
		if (shuttle.seats[j] == null){
		    //console.log("j: %d", j);
		    //console.log(placemarkers[i].getName());
		    //console.log(shuttle.seats.length);
		    seat_ok = true;
		    //shuttle.seats[j] = placemarkers[i].name 
			//+ " to " + placemarkers[i].house;
		    shuttle.seats[j] = {
			name: placemarkers[i].name,
			house: placemarkers[i].house
		    };
		    //console.log(shuttle.seats[j]);
		    $("#announcements").html(placemarkers[i].name + " picked up.");// + "Score: " + SCORE);
		    chart();
		    break;
		}
	    }
	    if ( seat_ok ) {
		
		// remove from placemarkers
		var features = earth.getFeatures();
		features.removeChild(placemarkers[i].pm);
		placemarkers.splice(i, 1);

		// remove from markers
		markers[i].setMap(null);
		markers.splice(i, 1);
		
	    } 
	    else {
		$("#announcements").html("Seats are full.");
	    }
	}
	else if ( dist <= 15.0 ){
	    // this is freshman, we do not know house.
	    // make announcement 
	    $("#announcements").html(placemarkers[i].name 
				     + " is freshman. Not allow pick up.");
	}
    }// placemaker loop
}

/**
 * Populates Earth with passengers and houses.
 */
function populate()
{
    // mark houses
    for (var house in HOUSES)
    {
        // plant house on map
        new google.maps.Marker({
            icon: "https://google-maps-icons.googlecode.com/files/home.png",
            map: map,
            position: new google.maps.LatLng(HOUSES[house].lat, HOUSES[house].lng),
            title: house
        });
    }

    // get current URL, sans any filename
    var url = window.location.href.substring(0, (window.location.href.lastIndexOf("/")) + 1);

    // scatter passengers
    for (var i = 0; i < PASSENGERS.length; i++)
    {
        // pick a random building
        var building = BUILDINGS[Math.floor(Math.random() * BUILDINGS.length)];

        // prepare placemark
        var placemark = earth.createPlacemark("");
        placemark.setName(PASSENGERS[i].name + " to " + PASSENGERS[i].house);

        // prepare icon
        var icon = earth.createIcon("");
        icon.setHref(url + "/img/" + PASSENGERS[i].username + ".jpg");

        // prepare style
        var style = earth.createStyle("");
        style.getIconStyle().setIcon(icon);
        style.getIconStyle().setScale(4.0);

        // prepare stylemap
        var styleMap = earth.createStyleMap("");
        styleMap.setNormalStyle(style);
        styleMap.setHighlightStyle(style);

        // associate stylemap with placemark
        placemark.setStyleSelector(styleMap);

        // prepare point
        var point = earth.createPoint("");
        point.setAltitudeMode(earth.ALTITUDE_RELATIVE_TO_GROUND);
        point.setLatitude(building.lat);
        point.setLongitude(building.lng);
        point.setAltitude(0.0);

        // associate placemark with point
        placemark.setGeometry(point);

        // add placemark to Earth
        earth.getFeatures().appendChild(placemark);

        // add marker to map
        var marker = new google.maps.Marker({
            icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/man.png",
            map: map,
            position: new google.maps.LatLng(building.lat, building.lng),
            title: PASSENGERS[i].name + " at " + building.name
        });

        // remember passenger's placemark and marker for pick-up's sake
	//alert(placemark.getGeometry().getLatitude());
	//alert(placemark.getGeometry().getLongitude());
	placemarkers.push({
	    name: PASSENGERS[i].name,
	    house: PASSENGERS[i].house,
	    pm: placemark
	});
	markers.push(marker);
    } // end for loop (passengers)
}

/**
 * Score & Time
 */
function Score(){
    if ( TIME == 0){
	alert("Timeout.");
	TIME -= 1;
    } else if ( TIME > 0 ){
	TIME -= 1;
    }
    var html = "Score: ";
    html += SCORE;
    html += ", Time: ";
    html += TIME;
    html += ", Vel: "; 
    html += shuttle.velocity;
    $("#score").html(html);
}

function End_Time(){
    alert("Timeout");
}

/**
 * Velocity Change
 */

function velup(){
    shuttle.velocity += 1;
}
function veldown(){
    // limit is 50
    if (shuttle.velocity > 0){
	shuttle.velocity -= 1;
    }
}


/**
 * Handler for Earth's viewchange event.
 */
function viewchange() 
{
    // keep map centered on shuttle's marker
    var latlng = new google.maps.LatLng(shuttle.position.latitude, shuttle.position.longitude);
    map.setCenter(latlng);
    bus.setPosition(latlng);
}

/**
 * Unloads Earth.
 */
function unload()
{
    google.earth.removeEventListener(earth.getView(), "viewchange", viewchange);
    google.earth.removeEventListener(earth, "frameend", frameend);
}
