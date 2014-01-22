var papersLight = papersLight || {};

papersLight.init = function() {
    // Static dialogs submit buttons
    $('#pl-login-button').click(papersLight.adminLogin);
    $('#pl-add-paper-button').click(papersLight.addPaperSubmit);
    $('#pl-remove-paper-button').click(papersLight.removePaperSubmit);

    // Initialize the page for user or admin
    $.getJSON('request.php?action=init', function(data) {
        if (data.username === '' || data.username === null) {
            var content =
                '<button class="btn btn-primary" data-toggle="modal" data-target="#pl-login">Admin</button>';

            $('#pl-toolbar').html(content);
            papersLight.isAdmin = false;
            papersLight.showAllPapers();
        } else {
            papersLight.createAdminPanel();
            papersLight.showAllPapers();
        }
    });
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
                alert('Failed to login');
            } else {
                $('#pl-login').modal('hide');
                papersLight.createAdminPanel();
                papersLight.showAllPapers();
            }
        }
    });
};

papersLight.createAdminPanel = function() {
    papersLight.isAdmin = true;

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
            papersLight.papers = data;
            papersLight.showPapers();
        }
    });
};

papersLight.showPapers = function() {
    var content =
        '<table class="table table-hover">' +
        '  <thead>' +
        '    <tr>' +
        '      <th>Year</th>' +
        '      <th>Title</th>' +
        '      <th>Authors</th>' +
        '      <th>Source</th>';

    if (papersLight.isAdmin) {
        content +=
        '      <th>Admin</th>';
    }

    content +=
        '    </tr>' +
        '  </thead>' +
        '  <tbody>';
    
    for (var i = 0; i < papersLight.papers.length; ++i) {
        var paper = papersLight.papers[i];
        content +=
        '    <tr>' +
        '      <td>' + paper['year'] + '</td>' +
        '      <td>' + paper['title'] + '</td>' +
        '      <td>' + paper['author'] + '</td>' +
        '      <td>' + paper['source'] + '</td>';

        if (papersLight.isAdmin) {
            content +=
            '  <td>' +
            '    <a class="pl-paper-edit" id="pl-paper-edit-' + i + '" href="#"><i class="icon-edit"></i></a>' +
            '    <a class="pl-paper-remove" id="pl-paper-remove-' + i + '" href="#"><i class="icon-remove"></i></a>' +
            '  </td>';
        }

        content +=
        '    </tr>';
    }

    content +=
        '  </tbody>' +
        '</table>';

    $('#pl-papers').html(content);

    if (papersLight.isAdmin) {
        $('.pl-paper-remove').click(function() {
            var ind = parseInt($(this).attr('id').slice(16), 10);
            papersLight.removePaper(ind);
        });

        $('.pl-paper-edit').click(function() {
            var ind = parseInt($(this).attr('id').slice(14), 10);
            papersLight.editPaper(ind);
        });
    }
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
                        '    <input class="pl-paper-req-fields" type="text" id="pl-add-paper-' + req[i] + '" placeholder="' + req[i] + '">' +
                        '    <span class="help-inline"></span>' +
                        '  </div>' +
                        '</div>';
                }
                for (i = 0; i < opt.length; ++i) {
                    content +=
                        '<div class="control-group">' +
                        '  <label class="control-label" for="pl-add-paper-' + opt[i] + '">' + opt[i] + '</label>' +
                        '  <div class="controls">' +
                        '    <input class="pl-paper-opt-fields" type="text" id="pl-add-paper-' + opt[i] + '" placeholder="' + opt[i] + '">' +
                        '  </div>' +
                        '</div>';
                }
                $('#pl-add-paper-fields').html(content);
                $('.pl-paper-req-fields').on('input', function() {
                    if ($(this).val() !== '') {
                        $(this).parents('.control-group').removeClass('error');
                        $(this).siblings('span').html('');
                    }
                });
            });
            $('#pl-add-paper-type').val('conference');
            $('#pl-add-paper-type').trigger('change');
        }
    });
    $('#pl-add-paper').modal('show');
};

papersLight.addPaperSubmit = function() {
    var paper = {};
    var isValid = true;
    $('.pl-paper-req-fields').each(function() {
        var key = $(this).attr("id").slice(13);
        var value = $(this).val();

        if (value === '' || value === null) {
            $(this).parents('.control-group').addClass('error');
            $(this).siblings('span').html('Please enter the ' + key);
            isValid = false;
        } else {
            paper[key] = value;
        }
    });

    if (!isValid) return;
    $('.pl-paper-opt-fields').each(function() {
        var key = $(this).attr("id").slice(13);
        var value = $(this).val();

        if (value !== '' && value !== null) {
            paper[key] = value;
        }
    })

    $.ajax({
        url: 'request.php?action=addpaper',
        type: 'POST',
        data: {
            type: $('#pl-add-paper-type').val(),
            paper: JSON.stringify(paper)
        },
        dataType: 'json',
        success: function(data) {
            if (data.error) {
                alert('Failed to add the paper');
            } else {
                $('#pl-add-paper').modal('hide');
                papersLight.showAllPapers();
            }
        }
    });
};

papersLight.removePaper = function(ind) {
    var paper = papersLight.papers[ind];
    $('#pl-remove-paper-info').html(JSON.stringify(paper));
    $('#pl-remove-paper').modal('show');
};

papersLight.removePaperSubmit = function() {
    var paper = JSON.parse($('#pl-remove-paper-info').text());

    $.ajax({
        url: 'request.php?action=removepaper',
        type: 'POST',
        data: {
            type: paper['type'],
            paper_id: paper['id']
        },
        dataType: 'json',
        success: function(data) {
            if (data.error) {
                alert('Failed to remove the paper');
            } else {
                $('#pl-remove-paper').modal('hide');
                papersLight.showAllPapers();
            }
        }
    });
};

papersLight.editPaper = function(ind) {

};

$(function() {
    papersLight.init();
});