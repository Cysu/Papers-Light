var papersLight = papersLight || {};

papersLight.init = function() {

    $.getJSON('request.php?action=init', function(data) {
        if (data.username === '' || data.username === null) {
            var content =
                '<button class="btn btn-primary btn-lg" data-toggle="modal" data-target="#pl-login">' +
                '  Admin' +
                '</button>';

            $('#pl-toolbar').html(content);
        } else {


        }
    });
};

papersLight.showAllPapers = function() {

    $.getJSON('request.php?action=getpapers', function(data) {

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
                '      <th>Source</th>' +
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
                '      <td>' + paper['source'] + '</td>' +
                '    </tr>';
            }

            content +=
                '  </tbody>' +
                '</table>';

            $('#pl-papers').html(content);
        }
    });
};

$(function() {
    papersLight.init();
    papersLight.showAllPapers();
});