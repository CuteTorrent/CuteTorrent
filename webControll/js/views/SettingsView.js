
_.templateSettings = {
    evaluate: /\{\{(.+?)\}\}/g,
    interpolate: /\{\{=(.+?)\}\}/g
};
window.SettingsView = Backbone.View.extend({
    model: new window.Settings,
    el:$('#SettingsConteiner'),
	events:{
		"click .list-group-item":"changeGroup",
		"click #groupUpdate":"groupUpdate",
		"click #groupDelete":"groupDelete"
	},
	groupDelete: function(e) {
		e.preventDefault();
		group = {
			name:$("#groupName").val(),
			extensions:$("#extentions").val(),
			path:$("#savePath").val()
		};
		groups=this.model.get('filteringGroups');
		found=false;
		for(var i=0;i<groups.length;i++)
		{
			if (group.name == groups[i].name)
			{
				groups.splice(i,1);
				break;
			}
		}
		
		this.model.set('filteringGroups',groups);
		this.model.save();
		this.render();
	},
	groupUpdate: function(e) {
		e.preventDefault();
		group = {
			name:$("#groupName").val(),
			extensions:$("#extentions").val(),
			path:$("#savePath").val()
		};
		groups=this.model.get('filteringGroups');
		found=false;
		for(var i=0;i<groups.length;i++)
		{
			if (group.name == groups[i].name)
			{
				groups[i]=group;
				found=true;
				break;
			}
		}
		if (!found)
		{
			groups.push(group);
		}
		this.model.set('filteringGroups',groups);
		this.model.save();
		this.render();
	},
	changeGroup: function(e) {
		groups=this.model.get('filteringGroups');
		currentGroupName = e.currentTarget.text;
		for(var i=0;i<groups.length;i++)
		{
			if (groups[i].name == currentGroupName)
			{
				$("#groupName").val(groups[i].name);
				$("#extentions").val(groups[i].extensions);
				$("#savePath").val(groups[i].path);
				break;
			}
		}
	},	
    initialize: function () {
        this.listenTo(this.model, 'change', this.render);
        this.model.fetch();
		this.template=$.templates("#settingsTmpl");
    },
	render: function () {
       $(this.el).html(this.template.render(this.model.attributes));
	   $('#prefernciestab a:first').tab('show');
		$('#prefernciestab a').click(function (e) {
			e.preventDefault();
			$(this).tab('show');
		})
        return this;

    }




});