_.templateSettings = {
    evaluate: /\{\{(.+?)\}\}/g,
    interpolate: /\{\{=(.+?)\}\}/g
};


window.RssListView = Backbone.View.extend({
    el:'#rss_tab',
	events:{
		"paste #searchRss" : "search",
		"keyup #searchRss" : "search",
		"click #removeFeed" : "removeFeed"
	},
	search: function(){
		this.render();
		var letters = $("#searchRss").val();
		$('#RssList').html('');
		this.collection.search(letters).forEach($.proxy(this.addOne,this));
	},
	removeFeed: function()
	{
		var openedAccordion = $('#RssList div.accordion-group .accordion-body.collapse.in');
		if (openedAccordion.length)
		{
			var id = openedAccordion.attr('id').replace('feedItems','');
			$('#ajax_loading').fadeIn(400);
			var rssView = this;
			$.ajax({
				url: '/rss/action',
				type: "POST",
				async:true,
				cache: false,
				data: {
					action: 'remove',
					feedId: id
				},
				success: function(jqXHR, textStatus, errorThrown) {
					$('#ajax_loading').fadeOut(400);
					rssView.updateCollection();
				}
			});
		}
	},
	updateCollection: function()
	{
		$('#ajax_loading').fadeIn(400);
		var page = this.paginationView.page_active;
		var rssListView = this;
		this.collection.fetch({
			data:{page:page,pageSize:10},
			success: function(collection, response, options){
				$('#ajax_loading').fadeOut(400);
				if (response.length>0)
				{
					rssListView.paginationView.page_count=Math.ceil(response[0].rssFeedsCount/rssListView.paginationView.page_show);
					rssListView.paginationView.render();
				}
				rssListView = null;
			},
			error: function(collection, response, options)	{
				$('#ajax_loading').fadeOut(400);
				errorData = {
					message:"Error while getting data from server"
				};
				$("#alertAria").append(rssListView.alertTemplate.render(errorData));
				$(".alert").alert();
				rssListView = null;
			}
		});
	},
	changePage: function(page)
	{
		if (typeof page == undefined)
		{
			page = 1;
		}
		$('#ajax_loading').fadeIn(400);
		var rssListView = this;
		this.collection.fetch({
			data:{page:page,pageSize:10},
			success: function(collection, response, options){
				$('#ajax_loading').fadeOut(400);
				if (response.length>0)
				{
					rssListView.paginationView.page_count=Math.ceil(response[0].rssFeedsCount/rssListView.paginationView.page_show);
					rssListView.paginationView.render();
				}
				rssListView = null;
			},
			error: function(collection, response, options)	{
				$('#ajax_loading').fadeOut(400);
				errorData = {
					message:"Error while getting data from server"
				};
				$("#alertAria").append(rssListView.alertTemplate.render(errorData));
				$(".alert").alert();
				rssListView = null;
			}
		});
	},
	initialize: function()
	{
		this.alertTemplate = $.templates("#alert-view");
		this.itemTemplate = $.templates("#rss-item-template");
		this.collection = new window.RssFeedCollection;
		this.paginationView = new window.PaginationView({link: '#rss/page/', el: '#rssPages'});
		var rssListView = this;
		this.collection.fetch({
			data:{page:1,pageSize:10},
			success: function(collection, response, options){
				if(response.length>0)
				{
					rssListView.paginationView.page_count=Math.ceil(response[0].rssFeedsCount/rssListView.paginationView.page_show);
					rssListView.paginationView.render();
				}
				rssListView = null;
				
			},
			error: function(collection, response, options)	{
				
				errorData = {
				
					message:"Error while getting data from server"
				};
				$("#alertAria").append(rssListView.alertTemplate.render(errorData));
				$(".alert").alert();
				rssListView = null;
			}
			});
		this.listenTo(this.collection, 'add', this.addOne);
		this.listenTo(this.collection, 'remove', this.removeOne);
		$('#RssList').html('');
	},
	removeOne: function(feed) {
		$('#feed'+feed.get('id'))
            .animate({ 'opacity': 0,'height':'0px'}, 800,function(){
					$(this).remove();
				});
	},
	addOne: function(feed) {
		$('#RssList').append(this.itemTemplate.render(feed.attributes));
	},
	addAll: function () {
        this.collection.forEach(this.addOne);
    },
});