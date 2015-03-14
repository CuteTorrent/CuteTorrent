(function ($) {
	$.fn.idle = function (onidle, onactive, options) {
		return this.each(function () {
			var isidle   = false,
			    hasMoved = false,
			    lastMove = (new Date()).getTime(),
			    opts;

			if ($.isPlainObject(onactive)) {
				options = onactive;
			}

			if (!$.isFunction(onactive)) {
				onactive = $.noop;
			}

			opts = $.extend({}, $.fn.idle.defaults, options);

			$(this).bind("mousemove", function () {
				hasMoved = true;
				lastMove = (new Date()).getTime();
				if (isidle) {
					onactive.call(this);
					isidle = false;
				}
			});

			window.setInterval(function () {
				if ((new Date()).getTime() - lastMove > opts.after) {
					if (hasMoved) {
						onidle.call(this);
					}
					lastMove = (new Date()).getTime();
					isidle = true;
				}
			}, opts.interval);
		});
	};

	// Set outside so they can be overrided
	// globally before being called on
	// an item
	$.fn.idle.defaults = {
		after: 5000,
		interval: 100
	};
}(jQuery));