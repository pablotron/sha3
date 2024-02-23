#!/usr/bin/env ruby
# frozen_string_literal: true

#
# Generate SHAKE128 and SHAKE256 test functions from response files in
# SHAKE byte test vectors archive (`shakebytetestvectors.zip`) from the
# Cryptographic Algorithm Validation Program (CAVP).
#
# Usage:
#   # download archive of byte test vectors
#   wget https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/sha3/shakebytetestvectors.zip
#
#   # run script to generate test functions on standard output
#   tests/gen-shake-tests.rb path/to/shakebytetestvectors.zip
#

# load libraries
require 'zip'

# number of elements per line of test data
PER_LINE = 128

# TODO
ALGOS = [{
  name: 'SHAKE128', # algo name
  fn: 'shake128', # function prefix

  # response files
  rsp_files: %w{
    SHAKE128ShortMsg.rsp
    SHAKE128LongMsg.rsp
    SHAKE128VariableOut.rsp
  },
}, {
  name: 'SHAKE256', # algo name
  fn: 'shake256', # function prefix

  # response files
  rsp_files: %w{
    SHAKE256ShortMsg.rsp
    SHAKE256LongMsg.rsp
    SHAKE256VariableOut.rsp
  },
}]

# test vector template
TEST_TMPL = <<END_TEST_TMPL
    { %<file_pos>d, %<line>d, %<src_ofs>d, %<src_len>d, %<exp_ofs>d, %<exp_len>d },
END_TEST_TMPL

# test function template
FN_TMPL = <<END_FN_TMPL
// %<name>s tests from shakebytetestvectors.zip
static void test_%<fn>s_xof_cmvp(void) {
  // data for tests
  static const uint8_t DATA[] = {
%<data>s
  }; // total size = %<data_size>d

  // response file names
  static const char *RSP_FILES[] = { %<rsp_files>s };

  // test vectors
  static const struct {
    const size_t rsp_file_ofs, // offset of source file name in RSP_FILES
                 rsp_file_line, // line within rsp file
                 src_ofs, // message offset into DATA
                 src_len, // message length of data
                 exp_ofs, // expected output offset into DATA
                 exp_len; // expected output length
  } TESTS[] = {
%<tests>s
  };

  // run tests
  for (size_t i = 0; i < sizeof(TESTS) / sizeof(TESTS[0]); i++) {
    // get expected result
    const uint8_t * const exp = DATA + TESTS[i].exp_ofs;
    const size_t exp_len = TESTS[i].exp_len;

    // hash data into "got"
    uint8_t got[%<max_exp_len>d] = { 0 };
    %<fn>s_xof_once(DATA + TESTS[i].src_ofs, TESTS[i].src_len, got, exp_len);

    // check for expected result
    if (memcmp(got, exp, exp_len)) {
      // get response file name and test vector line
      const char * const file_name = RSP_FILES[TESTS[i].rsp_file_ofs];
      const size_t file_line = TESTS[i].rsp_file_line;

      fprintf(stderr, "test_%<fn>s_xof_cmvp[%%zu, \\"%%s\\", line %%zu] failed:\\ngot = ", i, file_name, file_line);
      dump_hex(stderr, got,  exp_len);
      fprintf(stderr, "\\nexp = ");
      dump_hex(stderr, exp, exp_len);
      fprintf(stderr, "\\n");
    }
  }
}
END_FN_TMPL

# parse hash of key/value pairs into test vector row
def parse(file, h, data_size, &block)
  # parse hex-encoded message into array of bytes, then
  # truncate message to "Len" bytes, if "Len" is specified
  src = h[:Msg].scan(/../).map { |s| s.to_i(16) }
  src = src[0, h[:Len].to_i] if h.key?(:Len)
  src_ofs = data_size

  # parse hex-encoded expected output into array of bytes, then
  # truncate expected output to "OutputLen" bytes, if "OutputLen" is
  # specified
  exp = h[:Output].scan(/../).map { |s| s.to_i(16) }
  exp = exp[0, h[:OutputLen].to_i] if h.key?(:OutputLen)
  exp_ofs = data_size + src.size

  # append message bytes and expected output bytes to data
  block.call(src + exp)

  # return parsed entry
  {
    file: file,
    line: h[:line],

    # message
    src_ofs: src_ofs,
    src_len: src.size,

    # expected output
    exp_ofs: exp_ofs,
    exp_len: exp.size,
  }
end

# get zip path from first command-line argument
zip_path = ARGV.shift
raise "Usage #$0 <zip_path>" unless zip_path

# open zip file
Zip::File.open(zip_path, 'rb') do |zip|
  ALGOS.each do |algo|
    data = [] # test data (shared across all rsp files)

    # parse test vectors from rsp files
    rows = algo[:rsp_files].each_with_object([]) do |rsp_file, rows|
      curr = {} # current row

      # read lines from rsp file
      lines = zip.glob(rsp_file).first.get_input_stream.readlines.to_a.map { |line| line.strip }

      # parse lines into rows
      lines.size.times.each do |line_i|
        case lines[line_i]
        when /^(\w+) = (\w+)$/
          k, v = $1, $2 # extract key and value
          curr[k.intern] = v

          # cache line number
          curr[:line] = line_i unless curr[:line]
        when ''
          if curr.size > 0
            # parse current hash into test vector and add it to results
            rows << parse(rsp_file, curr, data.size) { |bytes| data += bytes }
          end

          curr = {} # clear current hash
        end
      end
    end

    # get maximum length of expected output
    # used to for size of "got" buffer in emitted code
    max_exp_len = rows.reduce(0) { |r, row| row[:exp_len] > r ? row[:exp_len] : r }

    # get maximum number of lines of emitted static DATA array
    num_lines = data.size / PER_LINE + ((data.size % PER_LINE) > 0 ? 1 : 0)

    # generate test function
    puts(FN_TMPL % {
      name: algo[:name], # algorithm name
      fn: algo[:fn], # algorithm function prefix

      # test vector data
      data: num_lines.times.map { |ofs|
        "    %s,\n" % [data[PER_LINE * ofs, PER_LINE].join(', ')]
      }.join,

      # response file names
      rsp_files: algo[:rsp_files].map { |file| '"%s"' % [file] }.join(', '),

      data_size: data.size, # total size of emitted DATA array, in bytes
      max_exp_len: max_exp_len, # maximum expected output length, in bytes

      # test vectors
      tests: rows.map { |row|
        TEST_TMPL % row.merge({ file_pos: algo[:rsp_files].index(row[:file]) })
      }.join,
    })
  end
end
