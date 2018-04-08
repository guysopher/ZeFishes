'use strict';
document.addEventListener('DOMContentLoaded', function() {
  var h1 = document.getElementsByTagName('h1');
  if (h1.length > 0) {
    h1[0].innerText = 'Ready';
  }

  console.log('DOMContentLoaded');

  init();

  const serial = new Serial();

}, false);

const init = () => {
}
