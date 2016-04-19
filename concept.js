var read_line = require('read-line')
var fs = require('fs')

var input = fs.createReadStream(__dirname + '/logfile.txt')
input.pipe(read_line)
read_line.on('readable', function() {
    var line
    while (line = read_line.read()) {
        var arr = line.toString().split(";")
        var strJson = {
            time: arr[0].replace(/\s/g, ''),
            type: arr[1].replace(/\s/g, ''),
            event_name: arr[2].replace(/\s/g, ''),
            event_description: arr[3].replace(/\s/g, ''),
            code: arr[6].replace(/\s/g, '')
        }
        console.log(strJson)
        // strJson to Graph using kibana
        // so have to use elastic search
    }
})
