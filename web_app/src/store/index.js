import Vue from "vue";
import Vuex from "vuex";
import * as mqtt from "mqtt";
Vue.use(Vuex);

const mqttUpdate = store => {
  const client = mqtt.connect("ws://192.168.1.150:1883"); // you add a ws:// url here
  client.subscribe("sensors/visual");

  client.on("message", function(topic, payload) {
    const data = JSON.parse(payload);
    console.log(data);
    if (data.event === "nir-clear") {
      store.dispatch("updateNIR", data.nir);
      store.dispatch("updateCLR", data.clr);
    } else if (data.event === "uv") {
      store.dispatch("updateUVI", data.uvi);
      store.dispatch("updateLux", data.lux);
    } else if (data.event === "spectrum") {
      for (let x = 1; x < 9; x++) {
        store.dispatch("updateWavelength", { wl: x, count: data[x] });
      }
    }
  });

  // // connect to mqtt here
  // store.subscribe((mutation, state) => {
  //   // called after every mutation.
  //   // The mutation comes in the format of `{ type, payload }`.
  // })
  //
};

export default new Vuex.Store({
  state: {
    uvi: 0,
    lux: 0,
    nir: 0,
    clr: 0,
    wlcount: {
      1: 400,
      2: 430,
      3: 448,
      4: 470,
      5: 540,
      6: 580,
      7: 690,
      8: 1100
    }
  },
  mutations: {
    UPDATE_UV(state, uvi) {
      state.uvi = Number.parseFloat(uvi);
    },
    UPDATE_LUX(state, lux) {
      state.lux = Number.parseFloat(lux);
    },    
    UPDATE_CLR(state, clr) {
      state.clr = Number.parseInt(clr);
    },
    UPDATE_NIR(state, nir) {
      state.nir = Number.parseInt(nir);
    },
    UPDATE_WL(state, { wl, count }) {
      state.wlcount[wl] = Number.parseInt(count);
    }
  },
  getters: {
    wlcount: state => {
      return [{ data: Object.values(state.wlcount) }];
    }
  },
  actions: {
    updateUVI(context, payload) {
      context.commit("UPDATE_UV", payload);
    },
    updateLux(context, payload) {
      context.commit("UPDATE_LUX", payload);
    },    
    updateNIR(context, payload) {
      context.commit("UPDATE_NIR", payload);
    },
    updateWavelength(context, { wl, count }) {
      context.commit("UPDATE_WL", { wl, count });
    },
    updateCLR(context, payload) {
      context.commit("UPDATE_CLR", payload);
    }
  },
  modules: {},
  plugins: [mqttUpdate]
});
