import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

satel_ns = cg.esphome_ns.namespace("satel_ca6")
SatelCA6 = satel_ns.class_("SatelCA6", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SatelCA6),
    cv.Required("clk_pin"): cv.int_,
    cv.Required("data_pin"): cv.int_,
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID],
                           config["clk_pin"],
                           config["data_pin"])
    yield cg.register_component(var, config)
