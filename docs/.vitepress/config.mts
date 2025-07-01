import { defineConfig } from 'vitepress'

export default defineConfig({
  title: "Tanc",
  description: "High performance C libraries",
  base: '/tanc/dist',
  outDir: './dist',
  ignoreDeadLinks: true,
  themeConfig: {
    nav: [
      { text: 'Guide', link: '/guide/introduction', activeMatch: '/guide/' },
      { text: 'Lib', link: '/lib/list', activeMatch: '/lib/' },
      { text: 'v0.1', link: '' },
      // { text: 'Roadmap', link: '/roadmap' }
    ],

    sidebar: {
      '/guide/': [
        {
          text: 'Introduction',
          link: '/guide/introduction',
        },
        {
          text: 'Quick start',
          link: '/guide/quick-start',
        },
      ],
      '/lib/': [
        {
          text: 'Data structure',
          collapsed: true,
          items: [{
            text: 'List',
            link: '/lib/list'
          },{
            text: 'Slist',
            link: '/lib/slist'
          }]
        },
        {
          text: 'Unit Test',
          link: '/lib/utest',
        }]
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/sun0day/tanc' }
    ]
  }
})
