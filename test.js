let transform = require("./build/Release/transform.node");
console.log(transform.WGS84ToGoogle({ x: 116.404, y: 39.915 }));
// { lon: 116.41024449916938, lat: 39.91640428150164 }
