const express = require("express");
const app = express();
const path = require("path");
const request = require('request');
const PORT = 3000;

app.use(express.static("public"));

app.get("/", (req, res) => {
    res.status(200).sendFile(path.join(__dirname, "public", "index.html"));
})

app.get("/joke", (req, res) => {
    var limit = 1;

    request.get({
        url: 'https://api.api-ninjas.com/v1/jokes?limit=' + limit,
        headers: {
            'X-Api-Key': '6hKeSsrrBfpedtbKDNbCeA==tukMyIgLPyDW0ABN'
        },
    }, function(error, response, body) {
        if(error) {
            res.status(400).send("Something went wrong");
            return console.error('Request failed:', error);
        } else if(response.statusCode != 200) {
            res.status(400).send("Something went wrong");
            return console.error('Error:', response.statusCode, body.toString('utf8'));
        } else {
            res.status(200).send(body);
        }
    });
})

app.listen(PORT, () => {
    console.log("Listening to", PORT);
})