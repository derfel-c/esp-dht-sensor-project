module.exports = mongoose => {
  var schema = mongoose.Schema(
      {
        temperature: String,
        humidity: String,
        location: String,
        createdAt: Number
      },
      {
       // timestamps: { currentTime: () => Math.floor(Date.now() / 1000) }, Not supported by MongoDB/Mongoose for the Raspberry Pi 2
        versionKey: false,
      }
    );

  const Sensordata = mongoose.model("sensordata", schema, "sensordata");
  return Sensordata;
};
