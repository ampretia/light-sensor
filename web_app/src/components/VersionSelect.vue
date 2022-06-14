<template>
  <div class="container">
    
      <b-field horizontal v-bind:label="title" position="is-left">
        <b-select placeholder="Select a build label" @input="update" expanded>
          <option
            v-for="option in versions"
            :value="option"
            :key="option.id"
            expanded
          >
            {{ option.label }} [{{ option.id }}]
          </option>
        </b-select>
      </b-field>
      <b-field horizontal label="Other">
        <b-input v-model="other" @input="updateOther" expanded></b-input>
      </b-field>
    
  </div>
</template>

<script>
export default {
  name: "VersionSelect",
  props: {
    versions: Array,
    title: String,
  },
  data() {
    return { other: "" };
  },
  methods: {
    updateOther(e) {
      this.$emit("update", { e: { id: e }, src: this });
    },
    update(e) {
      console.log(e);
      this.$emit("update", { e, src: this });
    },
  },
};
</script>


