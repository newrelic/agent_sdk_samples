{
	"targets": [
		{
			"target_name": "agentsdk",
			"sources": [
				"src/main.cc"
			],
			"include_dirs": [
				"<!(node -e \"require('nan')\")",
				"lib/agent_sdk/include",
			],
			"link_settings": {
				"libraries": [
					"-lnewrelic-collector-client",
					"-lnewrelic-transaction",
					"-lnewrelic-common"
				],
				"library_dirs": [
					"/Users/allie/Workspace/agent-sdk/agent-sdk/build/Release/lib"
				]
			}
		}
	]
}
