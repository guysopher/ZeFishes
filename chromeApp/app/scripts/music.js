// var fs = require('fs');
// import fs from 'fs';/
// console.log(fs);

const logit = function() {
  console.log(arguments[0],arguments[1],arguments[2],arguments);
}

const SAMPLE_SIZE = 2;

class Music {
  constructor() {
    this.initSets();
    this.initDom();
    this.render();
    this.playIdle();
  }

  initDom() {
    this.dom = $('#music_container');
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

  shuffleSet(s) {
    const dom = $(`#s${s}`);
    this.sets[s].tracks = _.shuffle(this.sets[s].tracks);
    this.renderSet(this.sets[s], s);
  }

  pauseAll(exceptSet, exceptTrack) {
    this.sets.forEach((set, s) => {
      set.tracks.forEach((track, t) => {
        const dom = $(`#s${s}`);
        if (s !== exceptSet) {
          if (dom.hasClass('playing-set')) {
            this.shuffleSet(s);
            dom.removeClass('playing-set');
          }
        } else {
          dom.addClass('playing-set');
        }
        if (s !== exceptSet || t !== exceptTrack) {
          this.pauseTrack(s, t);
        } else {
          this.playTrack(s, t);
        }
      });
    });
  }

  playNext(curSet, curTrack) {
    const nextTrack = curTrack + 1;
    const next = $(`#s${curSet}t${nextTrack}`);
    if (next) {
      this.playTrack(curSet, nextTrack);
    } else {
      this.playIdle();
    }
  }

  playSet(s) {
    this.pauseAll(s, 0);
    this.playTrack(s, 0);
  }

  playIdle() {
    this.playSet(0);
  }

  playTrack(s, t) {
    const dom = $(`#s${s}t${t}`);
    dom.parent().addClass('playing-track');
    if (dom[0])
      dom[0].play();
  }

  pauseTrack(s, t) {
    const dom = $(`#s${s}t${t}`);
    dom.parent().removeClass('playing-track');
    if (dom[0])
      dom[0].pause();
  }

  renderSet(set, s) {
    $(`#s${s}`).html(`
    <div class="card music-card">
      <div class="card-body">
        <h5 class="card-title">${set.name}</h5>
      </div>
      <ul class="list-group list-group-flush">
      ${set.tracks.map((track, t) => {
          return (t < SAMPLE_SIZE) ? `<li class="list-group-item">
          <p>${track}</p>
          <audio id="s${s}t${t}" src="../music/${set.name}/${track}" controls preload="auto"
            title="${track}" />
          </li>` : '';
        }).join('')}
      </ul>
      ${s > 0 ?
      `<div class="card-body">
        <button id="play_s${s}" class="btn btn-primary">Play Set</button>
        <button id="pause_s${s}" class="btn btn-light">Pause Set</button>
        <button id="suffle_s${s}" class="btn btn-light">Suffle Set</button>
      </div>` : ``}
    </div>
    `);
    $(`#play_s${s}`).click(() => this.playSet(s));
    $(`#pause_s${s}`).click(() => this.playIdle(s));
    $(`#suffle_s${s}`).click(() => this.shuffleSet(s));
    set.tracks.forEach((track, t) => {
      $(`#s${s}t${t}`).on('play', () => {
        this.pauseAll(s, t)
      });
      $(`#s${s}t${t}`).on('ended', () => this.playNext(s, t));
    });
  }

  render() {
    this.dom.html('');
    this.sets.forEach((set, s) => {
      this.dom.append(`
        <div id="s${s}"></div>
      `)
      this.shuffleSet(s);
    });
  }
}




