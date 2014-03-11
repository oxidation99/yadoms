/**
 * Created by Nicolas on 01/03/14.
 */

function EnumParameterHandler(i18nContext, paramName, content, currentValue) {
   assert(i18nContext !== undefined, "i18nContext must contain path of i18n");
   assert(paramName !== undefined, "paramName must be defined");
   assert(content !== undefined, "content must be defined");
   assert(content.values !== undefined, "values field must be defined");
   //TODO : restaurer un controle
   //assert(content.values.length >= 2, "values field must have at least two values");

   this.values = content.values;
   this.value = currentValue;

   if ((this.value === undefined) || (this.value == null) || (this.value == "")) {
      //we set the default value
      this.value = content.defaultValue;
   }

   //if it is still not defined we take the first item in the list
   if ((this.value === undefined) || (this.value == null) || (this.value == "")) {
      this.value = content.values[0];
   }

   this.name = content.name;
   this.paramName = paramName;
   this.description = content.description;
   this.i18nContext = i18nContext;
   this.content = content;
}

EnumParameterHandler.prototype.getDOMObject = function () {
   //we provide a SpinEdit
   var input = "<select " +
                        "class=\"form-control\" " +
                        "id=\"" + this.paramName + "\" " +
                        "data-content=\"" + this.description + "\"" +
                        "required ";
   var i18nOptions = " i18n-options=\"";
   var i18nData = " data-i18n=\"";

   i18nData += "[data-content]" + this.i18nContext + this.paramName + ".description";

   i18nOptions += "\" ";
   i18nData += "\" ";
   input += i18nData + i18nOptions;
   input += "value =\"" + this.value + "\" >";

   //we iterate through the vlues collection
   $.each(this.values, function (key, value) {
      debugger;
      input += "<option value=\"" + key + "\" data-i18n=\"" + this.i18nContext + this.paramName + ".values." + key + "\">" + value + "</option>\n";
   });

   input += "</select>";

   var self = this;
   return ConfigurationHelper.createControlGroup(self, input);
};

EnumParameterHandler.prototype.getParamName = function() {
  return this.paramName;
};

EnumParameterHandler.prototype.getCurrentConfiguration = function () {
   this.value = parseInt($("select#" + this.paramName).val());
   return this.value;
};