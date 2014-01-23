var papersLight = papersLight || {};

papersLight.init = function() {
    papersLight.curEditPaper = {};

    // Static dialogs submit buttons
    $('#pl-login-button').click(papersLight.adminLogin);
    $('#pl-edit-paper-button').click(papersLight.editPaperSubmit);
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
            papersLight.isAdmin = true;
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
                papersLight.isAdmin = true;
                papersLight.createAdminPanel();
                papersLight.showAllPapers();
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

papersLight.getDislpayInfo = function(paper) {
    var info = {};

    info['year'] = ('year' in paper) ? paper['year'] : 'Unknown';

    info['title'] = ('title' in paper) ? paper['title'] : 'Unknown';

    info['author'] = ('author' in paper) ? paper['author'] : (
                     ('editor' in paper) ? paper['editor'] : 'Unknown');

    info['source'] = ('booktitle' in paper) ? paper['booktitle'] : (
                     ('journal' in paper) ? paper['journal'] : (
                     ('publisher' in paper) ? paper['publisher'] : 'Unknown'));

    return info;
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
        var paper = papersLight.getDislpayInfo(papersLight.papers[i]);
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
        $('.pl-paper-edit').click(function() {
            var ind = parseInt($(this).attr('id').slice(14), 10);
            papersLight.editPaper(ind);
        });

        $('.pl-paper-remove').click(function() {
            var ind = parseInt($(this).attr('id').slice(16), 10);
            papersLight.removePaper(ind);
        });
    }
};

papersLight.initEditPaperModal = function(data) {
    var content =
        '<div class="control-group">' +
        '  <label class="control-label" for="pl-edit-paper-type">Type</label>' +
        '  <div class="controls">' +
        '    <select id="pl-edit-paper-type">';

    for (var type in data) {
        if (!data.hasOwnProperty(type)) continue;
        content += '<option value="' + type + '">' + type + '</option>';
    }

    content +=
        '    </select>' +
        '  </div>' +
        '</div>' +
        '<div id="pl-edit-paper-fields">' +
        '</div>';

    $('#pl-edit-paper-form').html(content);

    $('#pl-edit-paper-type').change(function() {
        // Load the current editing paper
        var type = $('#pl-edit-paper-type').val();
        var req = data[type]['required'];
        var opt = data[type]['optional'];
        var content = '';
        for (var i = 0; i < req.length; ++i) {
            var key = req[i];
            content +=
                '<div class="control-group">' +
                '  <label class="control-label" for="pl-edit-paper-' + key + '">' + key + ' *</label>' +
                '  <div class="controls">' +
                '    <input class="pl-paper-req-fields" type="text" id="pl-edit-paper-' + key + '" placeholder="' + key + '"';

            if (key in papersLight.curEditPaper && papersLight.curEditPaper[key] !== null) {
                content += ' value="' + papersLight.curEditPaper[key] + '"';
            }

            content += '>' +
                '    <span class="help-inline"></span>' +
                '  </div>' +
                '</div>';
        }
        for (i = 0; i < opt.length; ++i) {
            var key = opt[i];
            content +=
                '<div class="control-group">' +
                '  <label class="control-label" for="pl-edit-paper-' + key + '">' + key + '</label>' +
                '  <div class="controls">' +
                '    <input class="pl-paper-opt-fields" type="text" id="pl-edit-paper-' + key + '" placeholder="' + key + '"';

            if (key in papersLight.curEditPaper && papersLight.curEditPaper[key] !== null) {
                content += ' value="' + papersLight.curEditPaper[key] + '"';
            }

            content += '>' +
                '  </div>' +
                '</div>';
        }
        $('#pl-edit-paper-fields').html(content);

        // Save the current editing paper
        $('.pl-paper-req-fields').on('input', function() {
            var key = $(this).attr('id').slice(14);
            var value = $(this).val();

            papersLight.curEditPaper[key] = value;

            if (value !== '') {
                $(this).parents('.control-group').removeClass('error');
                $(this).siblings('span').html('');
            }
        });
        $('.pl-paper-opt-fields').on('input', function() {
            var key = $(this).attr('id').slice(14);
            var value = $(this).val();

            papersLight.curEditPaper[key] = value;
        });
    });
};

papersLight.addPaper = function() {
    // Set paper_id to zero for new paper
    papersLight.curEditPaper = {paper_id: '0'};

    $('#pl-edit-paper-modal').html('Add Paper');

    $.getJSON('request.php?action=gettypes', function(data) {
        if (data.error) {
            $('#pl-edit-paper-form').html(data.error);
        } else {
            papersLight.initEditPaperModal(data);

            // Set default type to conference
            $('#pl-edit-paper-type').val('conference');
            $('#pl-edit-paper-type').trigger('change');
        }
    });

    $('#pl-edit-paper').modal('show');
};

papersLight.editPaper = function(ind) {
    // Papers in database always have their paper_id greater than zero
    papersLight.curEditPaper = $.extend({}, papersLight.papers[ind]);

    $('#pl-edit-paper-modal').html('Edit Paper');

    $.getJSON('request.php?action=gettypes', function(data) {
        if (data.error) {
            $('#pl-edit-paper-form').html(data.error);
        } else {
            papersLight.initEditPaperModal(data);

            // Set the correct paper type
            $('#pl-edit-paper-type').val(papersLight.curEditPaper.type);
            $('#pl-edit-paper-type').trigger('change');
        }
    });

    $('#pl-edit-paper').modal('show');
};

papersLight.editPaperSubmit = function() {
    var paper = {};
    var isValid = true;
    $('.pl-paper-req-fields').each(function() {
        var key = $(this).attr('id').slice(14);
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
        var key = $(this).attr('id').slice(14);
        var value = $(this).val();

        if (value !== '' && value !== null) {
            paper[key] = value;
        }
    })

    // Judge adding or editing
    if (papersLight.curEditPaper.paper_id === '0') {
        $.ajax({
            url: 'request.php?action=addpaper',
            type: 'POST',
            data: {
                type: $('#pl-edit-paper-type').val(),
                paper: JSON.stringify(paper)
            },
            dataType: 'json',
            success: function(data) {
                if (data.error) {
                    alert('Failed to add the paper');
                } else {
                    $('#pl-edit-paper').modal('hide');
                    papersLight.showAllPapers();
                }
            }
        });
    } else {
        $.ajax({
            url: 'request.php?action=updatepaper',
            type: 'POST',
            data: {
                orig_type: papersLight.curEditPaper.type,
                new_type: $('#pl-edit-paper-type').val(),
                paper_id: papersLight.curEditPaper.paper_id,
                paper: JSON.stringify(paper)
            },
            dataType: 'json',
            success: function(data) {
                if (data.error) {
                    alert('Failed to update the paper');
                } else {
                    $('#pl-edit-paper').modal('hide');
                    papersLight.showAllPapers();
                }
            }
        });
    }
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
            type: paper.type,
            paper_id: paper.paper_id
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

$(function() {
    papersLight.init();
});