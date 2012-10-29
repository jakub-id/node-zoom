exports = module.exports = {};

var errors = {
	'SystemError': 'low-level error',
	'QueryError': '查詢語法錯誤',
	'Bib1Error': ''
};

function __create(_name, _message){
	var CustomError = function(message){
		Error.call(this);
		Error.captureStackTrace(this, this.constructor);
		this.message = message || _message;
		this.name = _name;
	};
	CustomError.prototype.__proto__ = Error.prototype;
	return CustomError;
};

for(var i in errors){
	exports[i] = __create(i, errors[i]);
}
