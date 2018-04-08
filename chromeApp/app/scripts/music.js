// var fs = require('fs');
// import fs from 'fs';/
// console.log(fs);

const logit = function() {
  console.log(arguments[0],arguments[1],arguments[2],arguments);
}



class Music {
  constructor() {
    $('#music_files').change(logit)
  }
}
