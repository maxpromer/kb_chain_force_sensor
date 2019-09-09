Blockly.JavaScript['kb_chain_force_sensor_get_raw'] = function(block) {
	var dropdown_ch = block.getFieldValue('ch');
	var code = 'DEV_I2C1.I2C_FORCE_SENSOR(0, 0x2A).getRaw(' + dropdown_ch + ')';
	return [code, Blockly.JavaScript.ORDER_NONE];
};
