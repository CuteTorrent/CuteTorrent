/**
 * Created with IntelliJ IDEA.
 * User: rfedos
 * Date: 7/24/13
 * Time: 1:54 PM
 * To change this template use File | Settings | File Templates.
 */
_.templateSettings = {
    evaluate: /\{\{(.+?)\}\}/g,
    interpolate: /\{\{=(.+?)\}\}/g
};
window.TorrentListView = Backbone.View.extend({
    model: new window.Torrent,
    el:'#TorrentList',
	events:{
		"paste #search" : "search",
		"keyup #search" : "search",
		"click #pauseTorrent":"pauseTorrent",
		"click #startTorrent":"startTorrent",
		"click #removeTorrent":"removeTorrent",
	},
	alertId:0,
	pauseTorrent: function () {
		var torrentListView=this;
		if ($('body').find(".accordion-body.in").length)
			$('body').find(".accordion-body.in").each(function() {
				var id=$(this).attr('id').substring(19);
				var torrentModel=torrentListView.collection.get(id);
				$('#ajax_loading').fadeIn(400);
				$.ajax({
					url: '/torrent/action',
					type: "POST",
					async:true,
					cache: false,
					data : {action : "pause", id : id},
					success: function(jqXHR, textStatus, errorThrown)	{
						$('#ajax_loading').fadeOut(400);
						torrentModel.set('isPaused',true);
					},
					error: function(jqXHR, textStatus, errorThrown)	{
						$('#ajax_loading').fadeOut(400);
						lastId=torrentListView.alertId;
						torrentListView.alertId++;
						errorData = {
							id:lastId,
							message:textStatus
						};
						$("#alertAria").append(torrentListView.alertTemplate.render(errorData));
						$(".alert").alert();	
					}
				});
			
			});
		
		
    },
	search: function(){
		this.render();
		var letters = $("#search").val();
		this.collection.search(letters).forEach(this.addOne);
	},
	startTorrent: function () {
	var torrentListView=this;
	if ($('body').find(".accordion-body.in").length)
		$('body').find(".accordion-body.in").each(function() {
			var id=$(this).attr('id').substring(19);
			var torrentModel=torrentListView.collection.get(id);
			$('#ajax_loading').fadeIn(400);
			$.ajax({
					url: '/torrent/action',
					type: "POST",
					async:true,
					cache: false,
					data : {action : "resume", id : id},
					success: function(jqXHR, textStatus, errorThrown)	{
						$('#ajax_loading').fadeOut(400);
						torrentModel.set('isPaused',false);
					},
					error: function(jqXHR, textStatus, errorThrown)	{
						$('#ajax_loading').fadeOut(400);
						lastId=torrentListView.alertId;
						torrentListView.alertId++;
						errorData = {
							id:lastId,
							message:textStatus
						};
						$("#alertAria").append(torrentListView.alertTemplate.render(errorData));
						$(".alert").alert();
					}
				});
			
		//	torrentModel.save();
		});
    },
	removeTorrent: function () {
	var torrentListView=this;
	if ($('body').find(".accordion-body.in").length)
		$('body').find(".accordion-body.in").each(function() {
			var id=$(this).attr('id').substring(19);
			var torrentModel=torrentListView.collection.get(id);
			//torrentModel.set('torrentStatus',2);
			$('#ajax_loading').fadeIn(400);
			$.ajax({
					url: '/torrent/action',
					type: "POST",
					async:true,
					cache: false,
					data : {action : "remove", id : id},
					success: function(jqXHR, textStatus, errorThrown)	{
						$('#ajax_loading').fadeOut(400);
						torrentListView.collection.remove([torrentModel])
					},
					error: function(jqXHR, textStatus, errorThrown)	{
						$('#ajax_loading').fadeOut(400);
						lastId=torrentListView.alertId;
						torrentListView.alertId++;
						errorData = {
							id:lastId,
							message:textStatus
						};
						$("#alertAria").append(torrentListView.alertTemplate.render(errorData));
						$(".alert").alert();	
					}
				});
		//	torrentModel.destroy();
		});
    },
    initialize: function () {
		this.alertTemplate = $.templates("#alert-view");
        this.model.view = this;
		this.collection = new window.TorrentCollection;
		this.paginationView = new window.PaginationView;
		this.paginationView.view=this;
		var torrentListView=this;
		this.collection.fetch({
			data:{page:1,pageSize:10},
			success: function(collection, response, options){
				if(response.length>0)
				{
					torrentListView.paginationView.page_count=Math.floor(response[0].torrentsCount/torrentListView.paginationView.page_show+1);
					torrentListView.paginationView.render();
				}
			},
			error: function(collection, response, options)	{
				lastId=torrentListView.alertId;
				torrentListView.alertId++;
				errorData = {
					id:lastId,
					message:"Error while getting data from server"
				};
				$("#alertAria").append(torrentListView.alertTemplate.render(errorData));
				$(".alert").alert();
			}
			});
		this.listenTo(this.collection, 'add', this.addOne);
		this.listenTo(this.collection, 'remove', this.removeOne);
    },
	changePage: function(page) {
		if(this.updatingJQXHR)
		{
			this.updatingJQXHR.abort();
		}
		var torrentListView=this;
		$('#ajax_loading').fadeIn(400);
		this.collection.fetch({
			data:{page:page,pageSize:10},
			success:function(){
				$('#ajax_loading').fadeOut(400);
			},
			error: function(collection, response, options)	{
				$('#ajax_loading').fadeOut(400);
				lastId=torrentListView.alertId;
				torrentListView.alertId++;
				errorData = {
					id:lastId,
					message:"Error while getting data from server"
				};
				$("#alertAria").append(torrentListView.alertTemplate.render(errorData));
				$(".alert").alert();
			}
		});
	},
	collectionChanged: function() {
		
		this.collection.forEach(this.reRender);
	},
	
	reRender: function(torrent) {
		var view = new window.TorrentView({
            model: torrent,
			listView: this
        });
		view.render();
		$('#torrentData'+view.model.get('id')+' a:first').tab('show');
	    $('#torrentData'+view.model.get('id')+' a').click(function (e) {
	        e.preventDefault();
	        $(this).tab('show');
			
	    })
	},
	
	updateCollection: function() {
	var torrentListView=this;
	$('#ajax_loading').fadeIn(400);
		this.updatingJQXHR=this.collection.fetch({
			data:{page:this.paginationView.page_active,pageSize:10},
			success:function(){
				$('#ajax_loading').fadeOut(400);
			},
			error: function(collection, response, options)	{
				if (options.xhr.statusText=='abort')
				{
					$('#ajax_loading').fadeOut(400);
					return;
				}				
				$('#ajax_loading').fadeOut(400);
				lastId=torrentListView.alertId;
				torrentListView.alertId++;
				errorData = {
					id:lastId,
					message:"Error while getting data from server"
				};
				$("#alertAria").append(torrentListView.alertTemplate.render(errorData));
				$(".alert").alert();
			}
			
		});
	},
	removeOne: function(torrent) {
		$('#torrent'+torrent.get('id')).parent('div')
            .animate({ 'opacity': 0,'height':'0px'}, 800,function(){
					$(this).remove();
				});
	},
	addOne: function(torrent) {
		var view = new window.TorrentView({
            model: torrent
        });
		$('#TorrentList').append(view.render().el);
		$('#torrentData'+view.model.get('id')+' a:first').tab('show');
	    $('#torrentData'+view.model.get('id')+' a').click(function (e) {
	        e.preventDefault();
	        $(this).tab('show');
			
	    });
		
	},
	addAll: function () {
        this.collection.forEach(this.addOne);
    },
    render: function () {
      
		
		$(this.el).html('');
        return this;

    }




});