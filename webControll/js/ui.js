$(document).ready(function () {
    
     var url ='/torrent/upload',
        uploadButton = $('<button/>')
            .addClass('btn btn-primary')
            .prop('disabled', true)
            .text('Processing...')
            .on('click', function () {
				
				path=$("#torrent-save-path").val();
				if (path.length > 0 )
				{
					$("#torrent-save-path").parent('div.input-group').removeClass('has-error');
					var $this = $(this),
						data = $this.data();
					$this
						.off('click')
						.text('Abort')
						.on('click', function () {
							$this.remove();
							data.abort();
						});
					data.submit().always(function () {
						$this.remove();
					});
				}
				else
				{
					$("#torrent-save-path").parent('div.input-group').addClass('has-error');
				}
            });
    $('#fileupload').fileupload({
        url: url,
        dataType: 'json',
        autoUpload: false,
        acceptFileTypes: /(\.|\/)(torrent)$/i,
        maxFileSize: 5000000, // 5 MB
        // Enable image resizing, except for Android and Opera,
        // which actually support image resizing, but fail to
        // send Blob objects via XHR requests:
        disableImageResize: /Android(?!.*Chrome)|Opera/
            .test(window.navigator.userAgent),
        previewMaxWidth: 100,
        previewMaxHeight: 100,
        previewCrop: true
    }).on('fileuploadadd', function (e, data) {
        data.context = $('<div/>').appendTo('#files');
        $.each(data.files, function (index, file) {
            var node = $('<p/>')
                    .append($('<span/>').text(file.name));
            if (!index) {
                node
                    .append('<br>')
                    .append(uploadButton.clone(true).data(data));
            }
            node.appendTo(data.context);
        });
    }).on('fileuploadprocessalways', function (e, data) {
        var index = data.index,
            file = data.files[index],
            node = $(data.context.children()[index]);
        if (file.preview) {
            node
                .prepend('<br>')
                .prepend(file.preview);
        }
        if (file.error) {
            node
                .append('<br>')
                .append(file.error);
        }
        if (index + 1 === data.files.length) {
            data.context.find('button')
                .text('Upload')
                .prop('disabled', !!data.files.error);
        }
    }).on('fileuploadprogressall', function (e, data) {
        var progress = parseInt(data.loaded / data.total * 100, 10);
        $('#progress .progress-bar').css(
            'width',
            progress + '%'
        );
    }).on('fileuploaddone', function (e, data) {
        $.each(data.files, function (index, file) {
            var link = $('<a>')
                .attr('target', '_blank')
                .prop('href', file.url);
            $(data.context.children()[index])
                .wrap(link);
			$('#progress .progress-bar').css(
            'width',
             '0%'
			);
        });
    }).on('fileuploadfail', function (e, data) {
        $.each(data.files, function (index, file) {
            var error = $('<span/>').html(data.jqXHR.responseText);
            $(data.context.children()[index])
                .append(error).parent().addClass("has-error");
			$('#progress .progress-bar').css(
            'width',
             '0%'
			);
        });
    }).on('fileuploadsubmit', function (e, data) {
		data.formData = {savePath: $('#torrent-save-path').val()};
    }).prop('disabled', !$.support.fileInput)
        .parent().addClass($.support.fileInput ? undefined : 'disabled');
    $('#prefernciestab a:first').tab('show');

    $('#prefernciestab a').click(function (e) {
        e.preventDefault();
        $(this).tab('show');
    })
    $("li").tooltip({
        'selector': '',
        'placement': 'bottom'
    });
    $('.modal-footer').button()
    $('#prefernciestab a:first').tab('show');

    $('#prefernciestab a').click(function (e) {
        e.preventDefault();
        $(this).tab('show');
    })
    $("li").tooltip({
        'selector': '',
        'placement': 'bottom'
    });
    $('.modal-footer').button();

    $("#settingsSave").submit(function () {
		$.ajax({
				url: '/settings/api',
				type: "PUT",
				async:true,
				cache: false,
				data : JSON.stringify($('settingsSave').serializeObject()),
				success: function(msg){
				//	alert( "Прибыли данные: " + msg );
					},
					error: function(jqXHR, textStatus, errorThrown)	{
					//	alert( "error occured: " + msg );
					}
				});
        
        $('#preferenciesDialog').modal('hide')
        return false;
    });
    $("#magnetLink").submit(function () {
		$.ajax({
					url: '/magnet/api',
					type: "POST",
					async:true,
					cache: false,
					data : {
						magnetLink: $('#magnet-link').val(),
						savePath:$('#save-path').val()
					}
				
				});
        
        $('#magnetLinkOpen').modal('hide')
        return false;
    });
	var settingsView = new window.SettingsView;
	var torrentListView = new window.TorrentListView;
	torrentListView.render();
	var paginationController=new window.PaginationController({view:torrentListView});
	paginationController.view=torrentListView;
	Backbone.history.start();
    var updatingID = setInterval(function () {
        torrentListView.updateCollection();
    }, 10000);
    $("#content").idle(
        function () {
            clearInterval(updatingID);
            $("#content").fadeTo("slow", .1);
        },
        function () {
            torrentListView.updateCollection();
            updatingID = setInterval(function () {
                torrentListView.updateCollection();
            }, 10000);
            $("#content").fadeTo("fast", 1);
        }, {
            after: 30000
        }
    );
});