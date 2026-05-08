import esphome.codegen as cg
CONF_Z7 = "z7"
CONF_TROUBLE = "trouble"
CONF_STATE = "state"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SatelCA6),
        cv.Required(CONF_CLK_PIN): cv.int_,
        cv.Required(CONF_DATA_PIN): cv.int_,

        cv.Optional(CONF_Z1): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_Z2): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_Z3): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_Z4): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_Z5): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_Z6): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_Z7): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_TROUBLE): binary_sensor.binary_sensor_schema(),

        cv.Optional(CONF_STATE): text_sensor.text_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_CLK_PIN],
        config[CONF_DATA_PIN],
    )

    await cg.register_component(var, config)

    if CONF_Z1 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_Z1])
        cg.add(var.set_z1_sensor(sens))

    if CONF_Z2 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_Z2])
        cg.add(var.set_z2_sensor(sens))

    if CONF_Z3 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_Z3])
        cg.add(var.set_z3_sensor(sens))

    if CONF_Z4 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_Z4])
        cg.add(var.set_z4_sensor(sens))

    if CONF_Z5 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_Z5])
        cg.add(var.set_z5_sensor(sens))

    if CONF_Z6 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_Z6])
        cg.add(var.set_z6_sensor(sens))

    if CONF_Z7 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_Z7])
        cg.add(var.set_z7_sensor(sens))

    if CONF_TROUBLE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_TROUBLE])
        cg.add(var.set_trouble_sensor(sens))

    if CONF_STATE in config:
        sens = await text_sensor.new_text_sensor(config[CONF_STATE])
        cg.add(var.set_state_sensor(sens))
