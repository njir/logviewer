var read_line = require('read-line')
var fs = require('fs')

var input = fs.createReadStream(__dirname + '/log.txt')
input.pipe(read_line)
read_line.on('readable', function() {
    var line
    while (line = read_line.read()) {
        var arr = line.toString().split(" ")
        var strJson = {
            time: arr[0],
            type: arr[1],
            event_id: arr[2]
            // ... add
        }
        console.log(strJson)
        // strJson to Graph using kibana
        // so have to use elastic search
    }
})