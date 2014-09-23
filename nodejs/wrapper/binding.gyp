{
	"targets": [
		{
			"target_name": "agentsdk",
			"sources": [
				"main.cc"
			],
			"include_dirs": [
				"<!(node -e \"require('nan')\")",
				"/usr/local/include"
			],
			"link_settings": {
				"libraries": [
					"-lnewrelic-collector-client",
					"-lnewrelic-transaction",
					"-lnewrelic-common"
				],
				"library_dirs": [
					"/usr/local/lib"
				]
			}
		}
	]
}
