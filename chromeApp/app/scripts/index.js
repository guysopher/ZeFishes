'use strict';
document.addEventListener('DOMContentLoaded', function() {
  const app = new App();
}, false);

class App {
  constructor() {
    this.serial = new Serial((action, param) => this.handleAction(action, param));
    this.music = new Music();
  }

  init() {
  }

  handleAction(action, param) {
    switch (action) {
      case 'PLAY':
        this.music.playTrack(param, 0);
      case 'STOP':
      case 'IDLE':
    }

  }
}

