const express = require("express");
const app = express();
const path = require("path");
const PORT = 8080;

app.use(express.static('public'))

app.get("/", (req, res) => {
    res.status(200).sendFile(path.join(__dirname, "public", "index.html"));
})

app.listen(PORT, () => {
    console.log("Listening to PORT " + PORT);
})