'use strict';

// Listens for the app launching then creates the window
chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('../react/dist/index.html', {
    state: 'fullscreen'
  });
});
