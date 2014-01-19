$(function() {
    $.getJSON('request.php', {action: 'getpapers'}, function(data) {
        if (data.error) {
            $('#pl-all').html(data.error);
        } else {
            $('#pl-all').html(data['test_key']);
        }
    });
});