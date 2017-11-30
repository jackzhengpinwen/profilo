from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from ..codegen import Codegen
from ..codegen import SIGNED_SOURCE


class JavaEntryTypesCodegen(Codegen):

    def __init__(self, entries):
        super(JavaEntryTypesCodegen, self).__init__()
        self.entries = entries

    def preferred_filename(self):
        return 'EntryType.java'

    def generate(self):
        template = """
// Copyright 2004-present Facebook. All Rights Reserved.
// %%SIGNED_SOURCE%%

package com.facebook.loom.entries;

public class EntryType {

%%ENTRIES_FIELDS%%

%%NAMES_FIELD%%
}
""".lstrip()

        fields = self._generate_entries_fields()

        fields = Codegen.indent(fields)
        template = template.replace('%%ENTRIES_FIELDS%%', fields)
        template = template.replace('%%SIGNED_SOURCE%%', SIGNED_SOURCE)

        names = self._generate_names_field()
        names = Codegen.indent(names)
        template = template.replace('%%NAMES_FIELD%%', names)

        return template

    def _generate_entries_fields(self):
        template = 'public static final int {0.name} = {0.id};'
        name_id_fields = [template.format(x) for x in self.entries]
        name_id_fields = '\n'.join(name_id_fields)

        return name_id_fields

    def _generate_names_field(self):
        template = """
public static final String[] NAMES = {
%%NAME_STRINGS%%
};""".lstrip()

        name_strings = ['"{0.name}",'.format(x) for x in self.entries]
        name_strings = '\n'.join(name_strings)
        name_strings = Codegen.indent(name_strings)

        return template.replace('%%NAME_STRINGS%%', name_strings)