var asteroidCount;

function getMonthPlus(date) {
  var month = date.getMonth()+1;
  return month < 10 ? '0' + month : month; // ('' + month) for string result
}

function getDayPlus(date) {
  var day = date.getDate();
  return day < 10 ? '0' + day : day; // ('' + day) for string result
}

var d = new Date();
var todayDate = d.getFullYear() + "-" + getMonthPlus(d) + "-" + getDayPlus(d);

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function asteroidToday() {
  // Construct URL
  var url = "http://www.neowsapp.com/rest/v1/feed?start_date="+todayDate+"&end_date="+todayDate;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      var nearEarthObjects = json.near_earth_objects;
      if(nearEarthObjects.length==0) {
        console.log(" neo is emtpy ");
        asteroidCount = "0";
      } else {
        asteroidCount = nearEarthObjects[todayDate].length;
      }
      // var temperature = Math.round(json.main.temp - 273.15);
      console.log('Asteroid count is ' + asteroidCount);
    
    // Assemble dictionary using our keys
    var dictionary = {
      'ASTEROID_COUNT': asteroidCount
    };

    // Send to Pebble
    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log('asteroid data sent to Pebble successfully!');
      },
      function(e) {
        console.log('Error sending asteroid info to Pebble!');
      }
    );
  }
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');

    // Get the initial weather
    asteroidToday();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    asteroidToday();
  }                     
);


