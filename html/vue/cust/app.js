var express = require("express");
var bodyParser = require("body-parser");

var authRouter = require('./router/auth');
var custRouter = require('./router/cust');

var app = express();
app.use(express.static('public'));
app.get('/portal', function(req, res) {
	res.json({
		data: [{
			visits: 12,
			clicks: 100
		}, {
			location: 'shanghai',
			total: 17
		}]
	});
});

app.use(bodyParser.json());
app.use(authRouter);
app.use('/api', custRouter);

app.listen('9001', function() {
	console.log('server start at 9001.');
});
