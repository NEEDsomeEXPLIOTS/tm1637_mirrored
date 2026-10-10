from esphome import pins
import esphome.codegen as cg
from esphome.components import tm1637
import esphome.config_validation as cv
from esphome.const import CONF_ID


DEPENDENCIES = ['esp32', 'esp8266']
AUTO_LOAD = ['tm1637']

tm1637_mirrored_ns = cg.esphome_ns.namespace('tm1637_mirrored')
TM1637Display = tm1637_mirrored_ns.class_("TM1637Display", cg.PollingComponent)
TM1637DisplayRef = TM1637Display.operator("ref")

CONF_CLK_PIN = "clk_pin"
CONF_DIO_PIN = "dio_pin"
CONF_MIRROR_SEGMENTS = "mirror_segments"
CONF_REVERSE_DIGITS = "reverse_digits"
CONF_LENGTH = "length"
CONF_INTENSITY = "intensity"
CONF_UPDATE_INTERVAL = "update_interval"
CONF_INVERTED = "inverted"

# Build config schema extending TM1637 base
CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(TM1637MirroredDisplay),
        
        # Required TM1637 pins
        cv.Required(CONF_CLK_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_DIO_PIN): pins.gpio_output_pin_schema,
        
        # Optional TM1637 options
        cv.Optional(CONF_INVERTED, default=False): cv.boolean,
        cv.Optional(CONF_LENGTH, default=4): cv.int_range(min=1, max=6),
        cv.Optional(CONF_INTENSITY, default=7): cv.int_range(min=0, max=7),
        cv.Optional(CONF_UPDATE_INTERVAL, default='1s'): cv.positive_time_period_milliseconds,
        
        # Custom mirroring options
        cv.Optional(CONF_MIRROR_SEGMENTS, default=True): cv.boolean,
        cv.Optional(CONF_REVERSE_DIGITS, default=True): cv.boolean,
    })
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    
    # Configure TM1637 pins
    clk_pin = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_clk_pin(clk_pin))
    dio_pin = await cg.gpio_pin_expression(config[CONF_DIO_PIN])
    cg.add(var.set_dio_pin(dio_pin))
    
    # Configure standard TM1637 options
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    cg.add(var.set_length(config[CONF_LENGTH]))
    cg.add(var.set_intensity(config[CONF_INTENSITY]))
    
    # Configure custom mirroring options
    cg.add(var.set_mirror_segments(config[CONF_MIRROR_SEGMENTS]))
    cg.add(var.set_reverse_digits(config[CONF_REVERSE_DIGITS]))
    
    # Set update interval
    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))