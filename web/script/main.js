var papersLight = papersLight || {};

papersLight.showAllPapers = function() {

    $.getJSON('request.php', {action: 'getpapers'}, function(data) {
        if (data.error) {
            $('#pl-all').html(data.error);
        } else {
            var content = 
                '<table class="table table-hover">' + 
                '  <thead>' +
                '    <tr>' +
                '      <th>Year</th>' +
                '      <th>Title</th>' +
                '      <th>Authors</th>' +
                '      <th>BookTitle</th>' +
                '    </tr>' +
                '  </thead>' +
                '  <tbody>';

            for (var i = 0; i < data.length; ++i) {
                var paper = data[i];
                content +=
                '    <tr>' +
                '      <td>' + paper['year'] + '</td>' +
                '      <td>' + paper['title'] + '</td>' +
                '      <td>' + paper['author'] + '</td>' +
                '      <td>' + paper['booktitle'] + '</td>' +
                '    </tr>';
            }

            content +=
                '  </tbody>' +
                '</table>';

            $('#pl-all').html(content);
        }
    });
};

$(function() {
    papersLight.showAllPapers();
});