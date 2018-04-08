class Serial {

  constructor() {

    this.active = true;

    this.decoder = new TextDecoder("utf-8");
    this.records = [];
    this.serialWindow = $('#serial-window');

    this.initDom();

    setInterval(() => {
      this.addRecord("--------------------| " + (new Date).toString() + "|--------------------", false);
    }, 60 * 1000);

  }

  logit() {
    console.log(arguments[0],arguments[1],arguments[2],arguments);
  }

  decode(buf) {
    return this.decoder.decode(buf);
  }

  connect(port) {
    this.disconnect();
    chrome.serial.connect(port, {
      bitrate: 9600,
      persistent: true,
    }, (info) => {
      this.connectionId = info.connectionId;
      this.addRecord('[Opened] Connection #' + info.connectionId + ", Port: " + port + ', Bitrate: ' + info.bitrate);
    });

    this.listener = chrome.serial.onReceive.addListener((res) => {
      this.onReceive(res);
    });
  }

  onReceive(res) {
    const record = this.decode(res.data);
    const separator = '\n';

    this.pendingRecord += record;

    const splitted = this.pendingRecord.split(separator);
    if (splitted.length > 1) {
      this.addRecord(splitted[0]);
      this.pendingRecord = splitted[1];
    }
  }

  disconnect() {
    try {
      chrome.serial.disconnect(this.connectionId, (info) => {
        this.addRecord('[Closed] Connection #' + this.connectionId);
        this.connectionId = null;
      });
    } catch(e) {
      //
    }
  }

  addRecord(record, addTime = true) {
    this.handleRecordActionIfNeeded(record);
    if (addTime) {
      record = `[${moment().format('HH:mm:ss.SSS')}] ${record}`;
    }
    this.records = [record].concat(this.records.slice(0,9999));
    this.render();
  }

  handleRecordActionIfNeeded(record) {
    const regex = /!\[[A-Z\d\s_-]+\]/;
    let m;

    if ((m = regex.exec(record)) !== null) {
        // The result can be accessed through the `m`-variable.
        m.forEach((match, groupIndex) => {
          const _regex = /[^!\[]+[^\]]/;
          let _m;

          if ((_m = _regex.exec(match)) !== null) {
              // The result can be accessed through the `m`-variable.
              _m.forEach((_match, _groupIndex) => {
                this.addRecord(`Found action, group ${_match}`);
              });
          }
        });
    }
  }

  initDom() {
    chrome.serial.getDevices(function(devices) {
      for (var i = 0; i < devices.length; i++) {
          $('select#portList').append('<option value="' + devices[i].path + '">' + (devices[i].displayName ? (devices[i].displayName + ": ") : "") + devices[i].path + '</option>');
      }
    });

    // ui hook
    $('button#port_open').click(() => {
        var port = $('select#portList').val();
        this.connect(port);
    });

    $('button#port_close').click(() => {
        var port = $('select#portList').val();
        this.disconnect(port);
    });

    $('button#serial_stop').click(() => {
        this.active = false;
    });

    $('button#serial_start').click(() => {
        this.active = true;
    });

  }

  render() {
    if (!this.active) return;

    const dom  = this.serialWindow;
    dom.html('');
    this.records.forEach((data, idx) => {
      dom.append("<li>" + data + "</li>");
    });

  }
}
