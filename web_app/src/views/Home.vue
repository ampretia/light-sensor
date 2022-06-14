<template>
  <div class="home">
    <div class="section">
      <h1 class="title">Light Sensor Display</h1>
    </div>

    <div class="section">
      <div class="columns">
        <div class="column" style="height: 300px;">
          <VueSvgGauge :start-angle="-150" :end-angle="150" :value="lux" :separator-step="30000" :min="0" :max="30000"
            :gauge-color="[{ offset: 0, color: '#347AB0' },{ offset: 100, color: '#B04334' }]" :scale-interval="0">
            <div class="inner-text">
              <span><b>Lux</b> <br />{{ lux }}</span>
            </div>
          </VueSvgGauge>
        </div>

        <div class="column" style="height: 300px;">
          <VueSvgGauge :start-angle="-150" :end-angle="150" :value="uvi" :separator-step="1" :min="0" :max="12"
            :gauge-color="[{ offset: 0, color: '#347AB0' },
            { offset: 30, color: '#72B034' },
            { offset: 60, color: '#B08D34' },
            { offset: 100, color: '#B04334' }]" :scale-interval="0">
            <div class="inner-text">
              <span><b>UVIndex</b> <br />{{ uvi }}</span>
            </div>
          </VueSvgGauge>
        </div>
      </div>
      <div class="columns">
        <div class="column" style="height: 300px;">
          <VueSvgGauge :start-angle="-150" :end-angle="150" :value="clr" :separator-step="55000" :min="0" :max="55000"
            :gauge-color="[{ offset: 0, color: '#E2F1FF' }, { offset: 100, color: '#0085FF' }]" :scale-interval="0">
            <div class="inner-text">
              <span><b>Clear</b> <br />{{ clr }}</span>
            </div>
          </VueSvgGauge>
        </div>
        <div class="column" style="height: 300px;">
          <VueSvgGauge :start-angle="-150" :end-angle="150" :value="nir" :separator-step="35000" :min="0" :max="35000"
            :gauge-color="[{ offset: 0, color: '#F3DDD5' }, { offset: 100, color: '#EC3F01' }]" :scale-interval="0">
            <div class="inner-text">
              <span><b>Near-IR</b> <br />{{ nir }}</span>
            </div>
          </VueSvgGauge>
        </div>
      </div>
    </div>

    <div class="section">
      <div id="chart">
        <apexchart  height="480" :options="pieChartOptions" :series="series[0].data"></apexchart>
      </div>
    </div>
  </div>
</template>

<script>
// @ is an alias to /src
//const labels=["415mm","445nm","480nm","515nm","555nm","590nm","638nm","680nm"]
import "vue-json-pretty/lib/styles.css";
import { VueSvgGauge } from 'vue-svg-gauge'
export default {
  name: "Home",
  components: { VueSvgGauge },
  data() {
    return {
      chartOptions: {
        chart: {
          type: "bar",
          height: 380
        },
        plotOptions: {
          bar: {
            barHeight: "100%",
            distributed: true,
            horizontal: false,
            dataLabels: {
              position: "bottom"
            }
          },

        },
        dataLabels: {
          enabled: true,
          style: {
            fontSize: "12px",
            colors: [
              "#fefefe", "#fefefe", "#fefefe", "#777777", "#777777", "#777777", "#f3f3f3", "#fefefe"]
          }
        },
        colors: [
          "#8f3eff",
          "#4b3e82",
          "#3e3eff",
          "#3effff",
          "#3eff3e",
          "#EAEA3B",
          "#ffa53e",
          "#ff3e3e"
        ],
        legend: {
          show: false
        },
        stroke: {
          width: 1,
          colors: ["#fff"]
        },
        xaxis: {
          categories: [
            "415mm",
            "445nm",
            "480nm",
            "515nm",
            "555nm",
            "590nm",
            "638nm",
            "680nm"
          ]
        },
        yaxis: {
          labels: {
            show: false
          }
        },
        title: {
          text: "Wavelength counts",
          align: "center",
          floating: true
        },

        tooltip: {
          theme: "dark",
          x: {
            show: false
          },
          y: {
            title: {
              formatter: function () {
                return "";
              }
            }
          }
        }
      },
      pieChartOptions: {
        chart: {
          type: "pie",
          height: 380
        },
        dataLabels: {
          enabled: true,
          style: {
            fontSize: "12px",
            colors: [
              "#fefefe", "#fefefe", "#fefefe", "#777777", "#777777", "#777777", "#f3f3f3", "#fefefe"]
          },
          dropShadow: {
            enabled: false
          }
        },
        colors: [
          "#8f3eff",
          "#4b3e82",
          "#3e3eff",
          "#3effff",
          "#3eff3e",
          "#EAEA3B",
          "#ffa53e",
          "#ff3e3e"
        ],

        labels: [
          "415mm",
          "445nm",
          "480nm",
          "515nm",
          "555nm",
          "590nm",
          "638nm",
          "680nm"
        ]
        ,
        legend: {
          show: true, position: 'bottom'
        }

      }
    };
  },
  computed: {
    uvi: function () {
      return this.$store.state.uvi;
    },
    lux: function () {
      return this.$store.state.lux;
    },
    nir: function () {
      return this.$store.state.nir;
    },
    nirpc: function () {
      return this.$store.state.nir / 10;
    },
    clr: function () {
      return this.$store.state.clr;
    },
    clrpc: function () {
      return this.$store.state.clr / 20;
    },
    series: function () {
      return this.$store.getters.wlcount;
    }
  },
  methods: {}
};
</script>
<style scoped>
.inner-text {
  margin-top: 70px;
}
</style>
