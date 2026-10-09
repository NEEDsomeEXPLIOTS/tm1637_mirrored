import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import tm1637
from esphome.const import CONF_ID

DEPENDENCIES = ['esp32']
AUTO_LOAD = ['tm1637']

tm1637_mirrored_ns = cg.esphome_ns.namespace('tm1637_mirrored')
TM1637MirroredDisplay = tm1637_mirrored_ns.class_('TM1637MirroredDisplay', tm1637.TM1637Display)

CONF_MIRROR_SEGMENTS = 'mirror_segments'
CONF_REVERSE_DIGITS = 'reverse_digits'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TM1637MirroredDisplay),
    cv.Optional(CONF_MIRROR_SEGMENTS, default='true'): cv.boolean,
    cv.Optional(CONF_REVERSE_DIGITS, default='true'): cv.boolean,
}).extend(tm1637.TM1637_DISPLAY_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await tm1637.register_tm1637(var, config)
    
    cg.add(var.set_mirror_segments(config[CONF_MIRROR_SEGMENTS]))
    cg.add(var.set_reverse_digits(config[CONF_REVERSE_DIGITS]))