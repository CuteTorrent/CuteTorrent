
window.PaginationController = Backbone.Router.extend({
    routes: {
        'torrent/page/:page': 'changePage'
		
    },
	initialize: function(view){
		this.view=view;
	},	
	changePage: function(page){
		
			this.view.paginationView.page_active=parseInt(page);
			this.view.paginationView.render();
			this.view.changePage(parseInt(page));
		
	}

});
