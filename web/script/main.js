var papersLight = papersLight || {};

papersLight.init = function() {
    $('#pl-login-button').click(function() {
        papersLight.adminLogin();
    });

    $.getJSON('request.php?action=init', function(data) {
        if (data.username === '' || data.username === null) {
            var content =
                '<button class="btn btn-primary" data-toggle="modal" data-target="#pl-login">Admin</button>';

            $('#pl-toolbar').html(content);
        } else {
            papersLight.createAdminPanel();
        }
    });

    papersLight.showAllPapers();
};

papersLight.adminLogin = function() {
    $.ajax({
        url: 'request.php?action=adminlogin',
        type: 'POST',
        data: {
            username: $('#pl-login-username').val(),
            password: $('#pl-login-password').val()
        },
        dataType: 'json',
        success: function(data) {
            if (data.error) {
                alert('Login failed');
            } else {
                $('#pl-login').modal('hide');
                papersLight.createAdminPanel();
            }
        }
    });
};

papersLight.createAdminPanel = function() {
    var content =
        '<button class="btn" type="button" id="pl-admin-add">Add</button>';

    $('#pl-toolbar').html(content);

    $('#pl-admin-add').click(function() {
        papersLight.addPaper();
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

papersLight.addPaper = function() {
    $.getJSON('request.php?action=gettypes', function(data) {
        if (data.error) {
            $('#pl-add-paper-form').html(data.error);
        } else {
            var content =
                '<div class="control-group">' +
                '  <label class="control-label" for="pl-add-paper-type">Type</label>' +
                '  <div class="controls">' +
                '    <select id="pl-add-paper-type">';

            for (var type in data) {
                if (!data.hasOwnProperty(type)) continue;
                content += '<option value="' + type + '">' + type + '</option>';
            }

            content +=
                '    </select>' +
                '  </div>' +
                '</div>' +
                '<div id="pl-add-paper-fields">' +
                '</div>';

            $('#pl-add-paper-form').html(content);
            $('#pl-add-paper-type').change(function() {
                var type = $('#pl-add-paper-type').val();
                var req = data[type]['required'];
                var opt = data[type]['optional'];
                var content = '';
                for (var i = 0; i < req.length; ++i) {
                    content +=
                        '<div class="control-group">' +
                        '  <label class="control-label" for="pl-add-paper-' + req[i] + '">' + req[i] + ' *</label>' +
                        '  <div class="controls">' +
                        '    <input type="text" id="pl-add-paper-' + req[i] + '" placeholder="' + req[i] + '">' +
                        '  </div>' +
                        '</div>';
                }
                for (i = 0; i < opt.length; ++i) {
                    content +=
                        '<div class="control-group">' +
                        '  <label class="control-label" for="pl-add-paper-' + opt[i] + '">' + opt[i] + '</label>' +
                        '  <div class="controls">' +
                        '    <input type="text" id="pl-add-paper-' + opt[i] + '" placeholder="' + opt[i] + '">' +
                        '  </div>' +
                        '</div>';
                }
                $('#pl-add-paper-fields').html(content);
            });
            $('#pl-add-paper-type').val('conference');
            $('#pl-add-paper-type').trigger('change');
        }
    });

    $('#pl-add-paper').modal('show');
};

$(function() {
    papersLight.init();
});