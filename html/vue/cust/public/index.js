
var IndexComponent = {
		template: `
			<div>
				<h1>Index页面</h1>
				<p>服务端数据：{{stat}}</p>
			</div>
		`,
		data: function() {
			return {
				stat: ''
			}
		},
		methods: {
			getStat: function() {
				return axios.get('/portal');
			}
		},
		created: function() {
			this.getStat().then(res => {
				this.stat = JSON.stringify(res.data);
			}).catch(err => {
				console.log(err);
			});
		}
};

var CustsComponent = {
		template: `
			<div>
				<div>
					<input type="text" v-model="keyword" />
					<input type="button" @click="getCusts()" value="Query" />
				</div>
				<ul>
					<router-link v-for="c in custs" tag="li" :to="{name: 'detail', params: { id:c.id }}" :key="c.id">{{c.name}}</router-link>
				</ul>
			</div>
		`,
		data: function() {
			return {
				custs: [],
				keyword: ''
			}
		},
		created: function() {
			this.getCusts();
		},
		methods: {
			getCusts: function() {
				axios.get('/api/cust/list', {
					params: {
						keyword: this.keyword
					}
				}).then(res => {
					this.custs = res.data;
					console.log('response.', this.custs);
				}).catch(err => console.log(err));
			}
		}
};

var DetailComponent = {
		template: `
			<div>{{cust}}</div>
		`,
		data: function() {
			return {
				cust: {}
			}
		},
		created: function() {
			var id = this.$route.params.id;
			this.getCust(id);
		},
		watch: {
			'$route': function() {
				console.log(this.$route.params.id);
			}
		},
		methods: {
			getCust: function(id) {
				axios.get('/api/cust/detail/'+id)
					.then(res => this.cust=res.data)
					.catch(err => console.log(err));
			}
		}
};

var router = new VueRouter({
	routes: [{
		name: 'index',
		path: '/index',
		component: IndexComponent
	}, {
		name: 'custs',
		path: '/custs',
		component: CustsComponent
	}, {
		name: 'detail',
		path: '/detail/:id',
		component: DetailComponent
	}]
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