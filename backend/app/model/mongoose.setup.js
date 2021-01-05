const dbConfig = require("../config/db.config");
const mongoose = require("mongoose");

const db = {
  mongoose: mongoose,
  url: dbConfig.url,
  sensordata: require("./sensordata.model")(mongoose)
};

module.exports = db;

