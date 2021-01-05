const db = require("../model/mongoose.setup");
const Sensordata = db.sensordata;


exports.create = (req, res) => {
  if (!req.body.temp || !req.body.hum || !req.body.location) {
    res.status(400).send({ message: "Content can not be empty!" });
    return;
  }
  const newSensordata = new Sensordata({
    temperature: req.body.temp,
    humidity: req.body.hum,
    location: req.body.location,
    createdAt: Math.floor(Date.now() / 1000)
  });

  newSensordata
    .save(newSensordata)
    .then((data) => {
      res.send(data);
    })
    .catch((err) => {
      res.status(500).send({
        message:
          err.message ||
          "Unknown Error occured while creating a new Sensordata object",
      });
    });
};

exports.findAll = (req, res) => {
  Sensordata.find({})
    .then((data) => {
      res.send(data);
    })
    .catch((err) => {
      res.status(500).send({
        message:
          err.message ||
          "Unknown Error occured while executing findAll Sensordata objects",
      });
    });
};

exports.findInTimeframe = (req, res) => {
  const from = req.query.from ? req.query.from : 0;
  const to = req.query.to ? req.query.to : Math.floor(Date.now() / 1000);
  Sensordata.find({ createdAt: { $gt: from, $lt: to } })
    .then((data) => {
      res.send(data);
    })
    .catch((err) => {
      res.status(500).send({
        message:
          err.message ||
          "Unknown Error occured while executing find all Sensordata objects in the given timeframe",
      });
    });
};

exports.findByLocation = (req, res) => {
  if (!req.query.lcoation) {
    res.status(400).send({ message: "Content can not be empty!" });
    return;
  }
  const location = req.query.lcoation 
  Sensordata.find({ location: location })
    .then((data) => {
      res.send(data);
    })
    .catch((err) => {
      res.status(500).send({
        message:
          err.message ||
          "Unknown Error occured while executing find all Sensordata objects by lcoation",
      });
    });
};

