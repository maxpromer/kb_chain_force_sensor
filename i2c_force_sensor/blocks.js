Blockly.Blocks['kb_chain_force_sensor_get_raw'] = {
	init: function() {
		this.jsonInit({
			"message0": Blockly.Msg.KB_CHAIN_FORCE_SENSOR_GET_RAW_MESSAGE,
			"args0": [{
				"type": "field_dropdown",
				"name": "ch",
				"options": [
					[ "A0", "0" ],
					[ "A1", "1" ]
				]
			}],
			"output": "Number",
			"colour": 300,
			"tooltip": Blockly.Msg.KB_CHAIN_FORCE_SENSOR_GET_RAW_TOOLTIP,
			"helpUrl": ""
		});
	}
};
