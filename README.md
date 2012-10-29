NODE-ZOOM
=========

    zoom.Connection('192.83.186.170:210/INNOPAC', function(conn){
        var query  = new zoom.Query('@attr 1=4 台灣');
        conn.search(query, function(record){
            // Do Something
        });
    });



