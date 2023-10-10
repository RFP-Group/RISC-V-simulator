require 'yaml'
require 'optparse'
require 'erb'

INSTRUCTION_CLASS_NAME = 'Instruction'.freeze
DECODER_CLASS_NAME = 'Decoder'.freeze
DECODER_FUNCTION_PREFIX_NAME = 'decode'.freeze
IMMEDIATE_TYPE = 'Immediate_t'.freeze
CODED_REGISTER_NAMES = %w[rd rs1 rs2 rs3 rm].freeze

class ISA
	def initialize(isa_content)
		@fields = isa_content['fields']
		@instructions = isa_content['instructions']
		@decodertree = isa_content['decodertree']
	end

	def form_hex_mask(range)
		enu = (range['lsb']..range['msb'])
		meaning_value = (enu.sum { |num| 2**num }).to_s(16)
		value = meaning_value.rjust(8, '0')
		"0x#{value}"
	end

	def generate_tabs(tabs)
		"\t" * tabs
	end

	def generate_switch_case(nodes, string, prev_tab_level)
		string += "switch(ApplyMask<uint32_t, #{form_hex_mask(nodes['range'])}>(raw_inst)) {\n"
		nodes['nodes'].each do |node|
			string += "#{generate_tabs(prev_tab_level)}\tcase #{node[0]}: "
			if node[1]['range'].nil?
				string += " instr = #{get_func_name(node[1])};\n"
			else
				string = generate_switch_case(node[1], string, prev_tab_level + 1)
			end
			string += "#{generate_tabs(prev_tab_level)}\t\treturn instr;\n"
		end
		string += "#{generate_tabs(prev_tab_level)}}\n"
		string
	end

	def get_inst_name(node)
		node['mnemonic'].gsub('.', '_').upcase
	end

	def get_func_name(node)
		"#{DECODER_FUNCTION_PREFIX_NAME}_#{get_inst_name(node)}(raw_inst)"
	end

	def get_decl_class_func_name(node)
		prefix = "#{INSTRUCTION_CLASS_NAME} #{DECODER_CLASS_NAME}::#{DECODER_FUNCTION_PREFIX_NAME}_"
		"#{prefix}#{get_inst_name(node)}(uint32_t raw_inst)"
	end

	def get_decl_func_name(node)
		"#{INSTRUCTION_CLASS_NAME} #{DECODER_FUNCTION_PREFIX_NAME}_#{get_inst_name(node)}(uint32_t raw_inst)"
	end

	def reg?(field)
		CODED_REGISTER_NAMES.any?(field['name'])
	end

	def imm?(field)
		field['name'] == 'imm'
	end

	def get_reg(field)
		location = field['location']
		bits = location['bits']
		mask = location['mask']
		string = "\tinstr.#{field['name']} = "
		"#{string}ApplyMaskAndShift<#{mask}, #{bits.first['msb']}, #{bits.first['from']}>(raw_inst);\n"
	end

	def get_imm(field)
		string = ''
		location = field['location']
		bits = location['bits']
		names = ''
		if bits.length == 1
			chunk = bits.first
			string += "\tinstr.#{field['name']} = "
			string += "ApplyMaskAndShift<#{form_hex_mask(chunk)}, #{chunk['msb']}, #{chunk['from']}>(raw_inst);\n"
		else
			bits.each do |chunk|
				name = "#{field['name']}_#{chunk['from']}_#{chunk['to']}"
				names += ' | ' unless names.empty?
				names += name
				string += "\t#{IMMEDIATE_TYPE} #{name} = "
				string += "ApplyMaskAndShift<#{form_hex_mask(chunk)}, #{chunk['msb']}, #{chunk['from']}>"
				string += "(raw_inst);\n"
			end
			string += "\tinstr.#{field['name']} = (#{names});\n"
		end
		string
	end

	def get_specials_in_imm(specials)
		string = ''
		names = ''
		specials.each do |special_type|
			location = special_type['location']
			bits = location['bits']
			name = "#{special_type['name']}_#{bits.first['msb']}_#{bits.first['lsb']}"
			names += ' | ' unless names.empty?
			names += name
			string += "\t#{IMMEDIATE_TYPE} #{name} = ApplyMask<uint32_t, #{form_hex_mask(bits.first)}>(raw_inst);\n"
		end
		string + "\tinstr.imm = (#{names});\n"
	end

	def bind
		binding
	end
end

class Generator
	def initialize(template, isa_content)
		@template = template
		@isa_content = isa_content
	end

	def generate_file(generated_file_path)
		isa_for_generation = ISA.new(@isa_content)
		erb = ERB.new(@template)
		generated_file = File.open(generated_file_path, 'w')
		generated_file << erb.result(isa_for_generation.bind)
		generated_file.close
	end
end
options = {}

OptionParser.new do |opts|
	opts.banner = 'Usage: generator.rb [options]'

	opts.on('-r', '--root PATH', 'Path to project source root') { |v| options[:root_path] = v }
	opts.on('-t', '--template PATH', 'Path to template file') { |v| options[:template_path] = v }
	opts.on('-o', '--output PATH', 'Path to generated file') { |v| options[:output_path] = v }
end.parse!

isa_file = "#{options[:root_path]}/isa/isa.yaml"
template_path = options[:template_path]

isa_content = YAML.load_file(isa_file)
template = File.read(template_path)
generator = Generator.new(template, isa_content)

generated_file_path = options[:output_path]
generator.generate_file(generated_file_path)
