'use strict';

// Listens for the app launching then creates the window
chrome.app.runtime.onLaunched.addListener(function() {
  var width = 400;
  var height = 300;

  chrome.app.window.create('index.html', {
    id: (new Date).toString(),
    outerBounds: {
      width: screen.availWidth - 100,
      height: screen.availHeight - 100,
      left: 50,
      top: 50
    }
  });
});
