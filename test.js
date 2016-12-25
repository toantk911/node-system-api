var system = require('./build/Release/system_api.node');

setInterval(function () {
  console.log(system.getIdleTime());
  console.log(system.getActiveWindow());
}, 1000);
