module.exports = app => {
  const sensordataController = require("../controller/sensordata.controller"); 
  var router = require("express").Router();

  /**
   * Gets all sensor data objects from the DB
   */
  router.get("/", sensordataController.findAll);

  /**
   * Get all sensor data objects from the DB in the given timeframe
   * @param from start date
   * @param to end date
   */
  router.get("/timeframe", sensordataController.findInTimeframe)

  /**
   * Get all sensor data objects from the DB with the given location
   * @param location of the sensor
   */
  router.get("/location", sensordataController.findByLocation)

  /**
   * Receives sensor data and creates a new DB entry
   */
  router.post('/submit', sensordataController.create)

  app.use("/api/sensordata", router);
}