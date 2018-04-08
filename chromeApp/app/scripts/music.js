// var fs = require('fs');
// import fs from 'fs';/
// console.log(fs);

const logit = function() {
  console.log(arguments[0],arguments[1],arguments[2],arguments);
}



class Music {
  constructor() {
    this.initSets();
    this.render();
    this.playIdle();
  }

  initSets() {
    this.sets = [
      {
        name: "Idle",
        tracks: [
          'Jean-Pierre Rampal - J.S. Bach _ Suite N°2 BWV 1067 - 2. Double-Badinerie.mp3',
        ]
      },{
        name: "Forrest Gump",
        tracks: [
          '(01) Elvis Presley - Hound Dog.mp3.mp3',
          '(02) Duane Eddy - Bebel Rouser.mp3',
          '(03) Clarence Henry - But I Do .mp3'
        ]
      },{
        name: "goran bregovic - black cat white cat soundtrack",
        tracks: [
          '01 bubamara (main version).mp3',
          '02 duj sandale.mp3',
          '03 railway station.mp3'
        ]
      }
    ];
  }

  suffleSet(setId) {
    this.sets[setId].tracks = _.shuffle(this.sets[setId].tracks);
  }

  pauseAll(exceptSet, exceptTrack) {
    this.sets.forEach((set, s) => {
      set.tracks.forEach((track, t) => {
        if (s !== exceptSet || t !== exceptTrack) {
          $(`#s${s}t${t}`)[0].pause();
        }
      });
    });
  }

  playNext(currentSet, currentTrack) {
    const next = this.sets[currentSet].tracks[currentTrack + 1];
    if (next) {
      $(`#s${currentSet}t${currentTrack + 1}`)[0].play();
    } else {
      this.playIdle();
    }
  }

  playIdle() {
    $(`#s0t0`)[0].play();
  }

  render() {
    const dom = $('#music_container');
    dom.html('');
    this.sets.forEach((set, s) => {
      dom.append(`
      <div class="card music-card">
        <div class="card-body">
          <h5 class="card-title">${set.name}</h5>
        </div>
        <ul class="list-group list-group-flush">
        ${set.tracks.map((track, t) => {
            return `<li class="list-group-item">
            <p>${track}</p>
            <audio id="s${s}t${t}" src="../music/${set.name}/${track}" controls preload="auto"
              title="${track}" />
            </li>`;
          }).join('')}
        </ul>
        <div class="card-body">
          <a id="suffle_s${s}" class="card-link">Suffle</a>
        </div>
      </div>
      `);
      $(`#suffle_s${s}`).click(() => this.suffleSet(s));
      set.tracks.forEach((track, t) => {
        $(`#s${s}t${t}`).on('play', () => this.pauseAll(s, t));
        $(`#s${s}t${t}`).on('ended', () => this.playNext(s, t));
      });

    });
  }
}




