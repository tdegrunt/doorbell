'use strict';

// Setup API token for http://2apn.com account
var API_TOKEN = "";

var sys = require('util');
var rest = require('restler');
var express = require('express');

var app = express.createServer();

app.get('/doorbell', function (req, res) {

  var data = {"user_credentials": API_TOKEN,
    "notification[message]": "There is someone at the door!",
    "notification[long_message]": "There is someone at the door!",
    "notification[title]": "Doorbell",
    "notification[long_message_preview]": "There is someone at the door!",
    "notification[message_level]": "2",
    "notification[silent]": "0",
    "notification[action_loc_key]": "Ok",
    //"notification[run_command]": "http://www.google.com/",
    "notification[sound]": "17.caf"};

  var body = [];
  Object.keys(data).forEach(function (k) {
    body.push(k + "=" + data[k]);
  });

  rest.post('https://www.appnotifications.com/account/notifications.json', {
    data: body.join('&'),
  }).on('complete', function (data, response) {
    if (response.statusCode == 201) {
      // you can get at the raw response like this...
    }
  });
});

app.listen(3000);