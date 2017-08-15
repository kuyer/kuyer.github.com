var jwt = require('jsonwebtoken');

var validUsers = [{
	username: 'xiaoming',
	password: '111111'
}, {
	username: 'xiaogang',
	password: '111111'
}];

const secretKey = 'dhahr3uiudfu93u43i3uy43&*&$#*&437hjhfjdjhfdfjsy8&*&*JNFSJDJHH??>:LP';

var createToken = function(user) {
	return jwt.sign({
		data: user,
		exp: Math.floor(Date.now()/1000) + (60*60)
	}, secretKey);
}

var parseToken = function(token, callback) {
	jwt.verify(token, secretKey, function(err, result) {
		callback && callback(err, result);
	});
}

module.exports = function(req, res, next) {
	console.log(req.path);
	if(req.path === '/login') {
		var username = req.body.username;
		var password = req.body.password;
		var user = validUsers.filter(u => u.username===username && u.password===password)[0];
		if(user) {
			res.json({
				success: true,
				message: '登录成功',
				token: createToken(user)
			});
		} else {
			res.json({
				success: false,
				message: '用户名或密码不正确'
			});
		}
	} else {
		var token = req.get('Authorization');
		console.log(token);
		if(token) {
			parseToken(token, function(err, result) {
				if(err) {
					res.status(401).json({
						success: false,
						message: JSON.stringify(err)
					});
				} else {
					next();
				}
			});
		} else {
			res.status(401).json({
				success: false,
				message: '未授权'
			});
		}
	}
}
