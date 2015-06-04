/**
 * Created with IntelliJ IDEA.
 * User: rfedos
 * Date: 7/25/13
 * Time: 12:50 PM
 * To change this template use File | Settings | File Templates.
 */
_.templateSettings = {
    evaluate: /\{\{(.+?)\}\}/g,
    interpolate: /\{\{=(.+?)\}\}/g
};
window.PaginationView = Backbone.View.extend({
   
   
   
    page_count: 1, // кол-во страниц
    page_active: 1, // активная страница
    page_show: 10, // кол-во страниц в блоке видимости
    attributes: { // атрибуты элемента
        "class": "pagination"
    },
    
    

    initialize: function (options) { // конструктор
       this.template= _.template($("#pagination-template").html()); // шаблон
	   this.link = options && options.link ? options.link : '#items/page/';
	   this.el = $(options.el);
    },

    render: function () { // выдача
        if (this.page_count > 1) {
            var range = Math.floor(this.page_show / 2);
            var nav_begin = this.page_active - range;
            if (this.page_show % 2 == 0) { // Если четное кол-во
                nav_begin++;
            }
            var nav_end = this.page_active + range;

            if (nav_begin <= 2) {
                nav_end = this.page_show;
                if (nav_begin == 2) {
                    nav_end++;
                }
                nav_begin = 1;
            }

            if (nav_end >= this.page_count - 1) {
                if (this.page_count > this.page_show)
                    nav_begin = this.page_count - this.page_show + 1;
                if (nav_end == this.page_count - 1) {
                    nav_begin--;
                }
                nav_end = this.page_count;
            }
            var paginationData = {
                link: this.link,
                page_count: this.page_count,
                page_active: this.page_active,
                nav_begin: nav_begin,
                nav_end: nav_end

            };
            
            this.el.html(this.template(paginationData)); 
            
            
        } else
		{
			this.el.html(''); 
		}
    }
});