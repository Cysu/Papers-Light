var papersLight = papersLight || {};

papersLight.init = function() {
    papersLight.sortOpt = {key: 'year', order: 'desc'};
    papersLight.refreshPapers();
};

papersLight.sourceSimplify = function(source) {
    r = /.*\((.+)\)/.exec(source);
    if (r !== null && r.length > 1) return r[1];
    return source;
};

papersLight.getDislpayInfo = function(paper) {
    var info = {};

    info['year'] = ('year' in paper) ? paper['year'] : 'Unknown';

    info['title'] = ('title' in paper) ? paper['title'] : 'Unknown';

    info['author'] = ('author' in paper) ? paper['author'] : (
                     ('editor' in paper) ? paper['editor'] : 'Unknown');

    info['source'] = papersLight.sourceSimplify(
                     ('booktitle' in paper) ? paper['booktitle'] : (
                     ('journal' in paper) ? paper['journal'] : (
                     ('publisher' in paper) ? paper['publisher'] : 'Unknown')));

    return info;
};

papersLight.sort = function() {
    // Parse bibtex for display
    papersLight.rows = [];
    for (var i = 0; i < papersLight.papers.length; ++i) {
        var paper = papersLight.papers[i];
        papersLight.rows.push({
            ind: i,
            year: ('year' in paper) ? paper['year'] : 'Unknown',
            title: ('title' in paper) ? paper['title'] : 'Unknown',
            author: ('author' in paper) ? paper['author'] : (
                    ('editor' in paper) ? paper['editor'] : 'Unknown'),
            source: papersLight.sourceSimplify(
                    ('booktitle' in paper) ? paper['booktitle'] : (
                    ('journal' in paper) ? paper['journal'] : (
                    ('publisher' in paper) ? paper['publisher'] : 'Unknown')))
        });
    }

    // Sort rows
    papersLight.rows.sort(function(a, b) {
        var key = papersLight.sortOpt.key;
        var order = papersLight.sortOpt.order;

        // Unknown items are always placed bottom
        if (a[key] === 'Unknown' && b[key] === 'Unknown') return 0;
        if (a[key] === 'Unknown' && b[key] !== 'Unknown') return 1;
        if (a[key] !== 'Unknown' && b[key] === 'Unknown') return -1;

        if (a[key].toLowerCase() === b[key].toLowerCase()) return 0;
        if (a[key].toLowerCase() < b[key].toLowerCase()) return (order === 'asc') ? -1 : 1;
        return (order === 'asc') ? 1 : -1;
    });
};

papersLight.display = function() {
    var sortIcon = ' <i class="icon-chevron-';
    sortIcon += ((papersLight.sortOpt.order === 'asc') ? 'up' : 'down');
    sortIcon += '"></i>';

    var content =
        '<table class="table table-hover">' +
        '  <thead>' +
        '    <tr>' +
        '      <th class="pl-paper-header" id="pl-paper-header-year"><a href="#">Year' +
                    ((papersLight.sortOpt.key === 'year') ? sortIcon : '') +
        '      </a></th>' +
        '      <th class="pl-paper-header" id="pl-paper-header-source"><a href="#">Source' +
                    ((papersLight.sortOpt.key === 'source') ? sortIcon : '') +
        '      </a></th>' +
        '      <th class="pl-paper-header" id="pl-paper-header-title"><a href="#">Title' +
                    ((papersLight.sortOpt.key === 'title') ? sortIcon : '') +
        '      </a></th>' +
        '      <th class="pl-paper-header" id="pl-paper-header-author"><a href="#">Authors' +
                    ((papersLight.sortOpt.key === 'author') ? sortIcon : '') +
        '      </a></th>' +
        '    </tr>' +
        '  </thead>' +
        '  <tbody>';

    for (var i = 0; i < papersLight.rows.length; ++i) {
        var paper = papersLight.rows[i];
        content +=
        '    <tr>' +
        '      <td>' + paper['year'] + '</td>' +
        '      <td>' + paper['source'] + '</td>' +
        '      <td>' + paper['title'] + '</td>' +
        '      <td>' + paper['author'] + '</td>' +
        '    </tr>';
    }

    content +=
        '  </tbody>' +
        '</table>';

    $('#pl-papers').html(content);
    $('.pl-paper-header').css('white-space', 'nowrap');
    $('.pl-paper-header').click(function() {
        var header = $(this).attr('id').slice(16);
        var sortOpt = papersLight.sortOpt;
        if (sortOpt.key === header) {
            sortOpt.order = (sortOpt.order === 'asc') ? 'desc' : 'asc';
        } else {
            sortOpt.key = header;
            sortOpt.order = 'desc';
        }
        papersLight.sort();
        papersLight.display();
    });
};

papersLight.refreshPapers = function() {
    $.getJSON('request.php?action=getpapers', function(data) {
        if (data.error) {
            $('#pl-all').html(data.error);
        } else {
            papersLight.papers = data;
            papersLight.sort();
            papersLight.display();
        }
    });
};

$(function() {
    papersLight.init();
});