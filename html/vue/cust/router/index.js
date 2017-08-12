
var router = new VueRouter({
	
});

new Vue({
	el: '#app',
	router: router,
	template: `
		<div>
			<router-link :to="{name: 'index'}">Index</router-link>
			<router-link :to="{name: 'custs'}">Custs</router-link>
			<router-view></router-view>
		</div>
	`
});