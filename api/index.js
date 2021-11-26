var express = require('express');
var router = express.Router();


// Bot Setting
const TelegramBot = require('node-telegram-bot-api');
const token = '2133290397:AAHIgB1ZusJ-m2-TluhcnWMfZQNwfRPMk1U';
const bot = new TelegramBot(token, {polling: true});


let global_msg_id;
// Main Menu Bot
bot.onText(/\/start/, (msg) => {
    global_msg_id = msg.chat.id;
    bot.sendMessage(
        global_msg_id,
        `hello ${msg.chat.first_name}, welcome...\n
        click /show_url`
    );
});

bot.onText(/\/show_url/, (msg) => {
    global_msg_id = msg.chat.id;
    bot.sendMessage(
        global_msg_id,
        `
            https://esp-telebot.herokuapp.com/api/sensor/123/65/78 \n
            https://esp-telebot.herokuapp.com/api/test/cobacoba
        `
    );
});

bot.on('message', (msg) => {
  console.log(msg);
});


/* GET users listing. */
router.get('/', (req, res, next) => {
  res.json({
    "status": 202,
    "messgae": "Success"
  });
});

// https://esp-telebot.herokuapp.com/api/sensor/123/65/78
router.get('/sensor/:sensor1/:sensor2/:sensor3', (req, res, next) => {
  try {
      bot.sendMessage(
            global_msg_id, //msg.id
            `Pembacaan Sensor:: ${req.params.sensor1}, ${req.params.sensor2}, ${req.params.sensor3}`
     );
      res.json({
        "status": 202,
        "message": "Success",
        "data": {
          "sensor_1": req.params.sensor1 + (0.1) * req.params.sensor2,
          "sensor_2": req.params.sensor2 + (0.1) * req.params.sensor3,
          "sensor_3": req.params.sensor3 + (0.1) * req.params.sensor1
        }
      });
  } catch (err) {
      next(err);
  }
});

// https://esp-telebot.herokuapp.com/api/test/cobacoba
router.get('/test/:key', function(req, res, next){
    bot.sendMessage(
            global_msg_id, //msg.id
            `${req.params.key}`
    );
    res.json(req.params.key);
});


module.exports = router;
