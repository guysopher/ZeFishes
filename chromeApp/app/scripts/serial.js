class Serial {

  constructor(handleAction) {

    this.parent = {
      handleAction
    };

    this.active = true;

    this.decoder = new TextDecoder("utf-8");
    this.encoder = new TextEncoder();
    this.records = [];
    this.serialWindow = $('#serial-window');

    this.initDom();
    this.initChime();
  }

  initChime() {
    const fiveSecs = 5 * 1000;
    const now = Date.now();
    const chime = () => "=========> " + moment(Date.now()).format("dddd, MMMM Do YYYY, HH:mm:ss");
    setTimeout(() => {
      setInterval(() => {
        this.addRecord(chime(), false);
      }, fiveSecs);
      this.addRecord(chime(), false);
    }, (((parseInt(now / fiveSecs) + 1) * fiveSecs) - now));
  }

  decode(buf) {
    return this.decoder.decode(buf);
  }

  encode(buf) {
    return this.encoder.encode(buf);
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

  send(str) {
    if (str) {
      this.addRecord("Sending status: " + str);
      chrome.serial.send(this.connectionId, this.encode(str), () => {})
    }
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
    if (!this.active) return;

    const regex = /!\[[A-Z\d\s_-]+\]/;
    let m;

    if ((m = regex.exec(record)) !== null) {
        // The result can be accessed through the `m`-variable.
        m.forEach((match, groupIndex) => {
          const _regex = /[^!\[]+[^\]]/;
          let _m;

          if ((_m = _regex.exec(match)) !== null) {
              _m.forEach((_match, _groupIndex) => {
                this.handleAction(_match);
                this.addRecord(`Found action, group ${_match}`);
              });
          }
        });
    }
  }

  handleAction(actionStr) {
    const [action, param] = actionStr.split(' ');
    this.parent.handleAction(action, param);
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

    $('button#serial_clear').click(() => {
        this.records = [];
        this.render();
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
