var zoom = require('../build/Release/zoom')
 ,  error = require('./error');

exports = module.exports = {};

exports.Query = function(query){
	var qo;
	
	if(typeof query !== 'string'){
		throw new error.QueryError('這不是字串阿 囧');
	}
	
	try{
		qo = zoom.query.prefix(query);
	}catch(err){
		throw new error.QueryError(err.message);
	}
	
	return qo;
};

exports.Query.cql = function(query){
	var qo;
	
	if(typeof query !== 'string'){
		throw new error.QueryError('這不是字串阿 囧');
	}
	
	try{
		qo = zoom.query.cql(query);
	}catch(err){
		throw new error.QueryError(err.message);
	}
	
	return qo;
};

exports.Query.cql2rpn = function(query, conn){
	var qo;
	
	if(!conn || !conn.constructor || conn.constructor.name !== 'Connection'){
		throw new Error('第二個參數不是 Connection');
	}

	if(typeof query !== 'string'){
		throw new error.QueryError('這不是字串阿 囧');
	}
	
	try{
		qo = zoom.query.cql2rpn(query, conn);
	}catch(err){
		throw new error.QueryError(err.message);
	}
	
	return qo;
};

function getResultSet(resultset){
	var record = resultset.record,
	    records = resultset.records;

	resultset.record = function(index, callback){
		if(index > resultset.size()){
			throw new Error('index 超過 ResultSet 的大小');
		}
		process.nextTick(function(){
			callback(record.apply(resultset, [index]));
		});
	};

	resultset.records = function(start, num, callback){
		if(start + num - 1 > resultset.size()){
			throw new Error('index 超過 ResultSet 的大小');
		}
		process.nextTick(function(){
			callback(records.apply(resultset, [start, num]));
		});
	};

	return resultset;
};

function getConn(conn){
	var search = conn.search,
	    scan = conn.scan,
	    connect = conn.connect;

	conn.search = function(query, callback){
		if(!query || !query.constructor || query.constructor.name !== 'Query'){
			throw new Error('這不是 Query');
		}
		process.nextTick(function(){
			var resultset;
			try{
				resultset = search.apply(conn, [query]);
				query.destroy();
			}catch(err){
				throw new error.Bib1Error(err.message);
			}
			callback(resultset);
		});
	};
	
	conn.scan = function(query, callback){
		if(!query || !query.constructor || query.constructor.name !== 'Query'){
			throw new Error('這不是 Query');
		}
		process.nextTick(function(){
			var scanset;
			try{
				scanset = scan.apply(conn, [query]);
				query.destroy();
			}catch(err){
				throw new error.Bib1Error(err.message);
			}
			callback(scanset);
		});
	};

	conn.connect = function(host, port, database, user, pass){
		var regex = /^([\w\.]+)(\:\d+)?(\/\w+)?$/,
		    callback;
		
		switch(arguments.length){
			case 0:
				return;
			case 3:
				callback = typeof database === 'function'? database: function(){};
				if(typeof port === 'object'){
					user = port.port || undefined;
					pass = port.pass || undefined;
				}
			case 2:
				callback = callback || (typeof port === 'function'? port: function(){});
				var setting = host.match(regex);
				host = setting[1];
				port = setting[2]? parseInt(setting[2].substr(1), 10): 210;
				database = setting[3]? setting[3].substr(1): undefined;
				break;
		}
		
		if(typeof user === 'string' && typeof pass === 'string'){
			conn.option('user', user);
			conn.option('password');
		}

		if(typeof database === 'string'){
			conn.option('databaseName', database);
		}
		
		if(typeof host === 'string' && (!port || typeof port === 'number')){
			process.nextTick(function(){
				try{
					connect.apply(conn, [host, port]);
				}catch(err){
					callback(error.SystemError(err.message));
					return;
				}
				callback(conn);
			});
		}
	};
	
	return conn;
};

exports.Connection = function(host, port, database, user, pass){
	var conn = getConn(zoom.connection.create());
	conn.connect.apply(conn, arguments);
	return conn;
};
