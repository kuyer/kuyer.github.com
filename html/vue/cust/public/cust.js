var router = require('express').Router();
var db = require('./custData');

router.get('/cust/list', function(req, res) {
	var list = db.datas;
	list = list.filter(v=>v.name.indexOf(req.query.keyword) !== -1);
	res.json(list);
});

router.get('/cust/detail/:id', function(req, res) {
	var list = db.datas;
	var obj = list.filter(v=>v.id==req.params.id)[0];
	res.json(obj);
});

module.exports = router;